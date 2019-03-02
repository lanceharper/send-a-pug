console.log("Loading function");

const AWS = require("aws-sdk");

// AWS.config.update({ region: "us-west-2" });

const ddb = new AWS.DynamoDB({ apiVersion: "2012-08-10" });

exports.handler = async event => {
  let success = 0;
  let failure = 0;

  const streamerToLeaderboardEntryMap = new Map();

  let recordOutput = [];
  event.records.forEach(record => {
    try {
      const recordData = Buffer.from(record.data, "base64");

      let { streamer_id, user_id, PUG_COUNT, ...restRecord } = JSON.parse(
        recordData.toString("ascii")
      );

      console.log("new record", {
        streamer_id,
        user_id,
        PUG_COUNT,
        restRecord
      });

      if (!streamerToLeaderboardEntryMap.has(streamer_id)) {
        streamerToLeaderboardEntryMap.set(
          streamer_id,
          new Map([[user_id, PUG_COUNT]])
        );
      } else {
        streamerToLeaderboardEntryMap.get(streamer_id).set(user_id, PUG_COUNT);
      }

      success++;
      recordOutput.push({
        recordId: record.recordId,
        result: "Ok"
      });
    } catch (err) {
      failure++;
      console.error(err);
      recordOutput.push({
        recordId: record.recordId,
        result: "DeliveryFailed"
      });
    }
  });

  console.log("streamerToLeaderboardEntryMap", streamerToLeaderboardEntryMap);

  await streamerToLeaderboardEntryMap.forEach(
    async (userToScoreMap, streamer_id) => {
      try {
        const previousLeaderboardForSteamerDDBParams = {
          TableName: process.env.LEADERS_TABLE,
          Key: {
            streamer_id: { S: streamer_id }
          },
          ProjectionExpression: "streamer_id, leaderboard"
        };

        const { Item: previousLeaderboardDDBItem } = await ddb
          .getItem(previousLeaderboardForSteamerDDBParams)
          .promise();

        const previousLeaderboardForStreamer = previousLeaderboardDDBItem
          ? AWS.DynamoDB.Converter.unmarshall(previousLeaderboardDDBItem)
          : {
              streamer_id,
              leaderboard: []
            };

        console.log(
          "previousLeaderboardForStreamer",
          previousLeaderboardForStreamer
        );

        let leaderboard = [];

        userToScoreMap.forEach((PUG_COUNT, user_id) => {
          console.log("PUG_COUNT, user_id", { PUG_COUNT, user_id });

          console.log(
            "previousLeaderboardForStreamer.leaderboard",
            previousLeaderboardForStreamer.leaderboard
          );

          const previousLeaderboardPlayerEntry = previousLeaderboardForStreamer.leaderboard.find(
            ({ player }) => user_id === player
          );

          console.log(
            "previousLeaderboardPlayerEntry",
            previousLeaderboardPlayerEntry
          );

          leaderboard.push({
            player: user_id,
            score: previousLeaderboardPlayerEntry
              ? previousLeaderboardPlayerEntry.score + PUG_COUNT
              : PUG_COUNT
          });
        });

        // If there are previous scores for a streamer's user,
        // increase their total score by the amount returned over the last time window
        const updatedScoresForStreamer = {
          streamer_id,
          leaderboard
        };

        console.log("updatedScoresForStreamer", updatedScoresForStreamer);

        updatedScoresForStreamerPutParams = {
          TableName: process.env.LEADERS_TABLE,
          Item: AWS.DynamoDB.Converter.marshall(updatedScoresForStreamer)
        };

        console.log(
          "updatedScoresForStreamerPutParams",
          updatedScoresForStreamerPutParams
        );
        await ddb.putItem(updatedScoresForStreamerPutParams).promise();
      } catch (error) {
        console.error(`Failed to update table.`, error);
      }
    }
  );

  console.log(
    `Successfully delivered records ${success}, Failed delivered records ${failure}.`
  );

  console.log("output", recordOutput);

  return {
    records: recordOutput
  };
};
