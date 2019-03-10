const AWS = require("aws-sdk");

AWS.config.update({ region: "us-west-2" });

const ddb = new AWS.DynamoDB({ apiVersion: "2012-08-10" });

exports.handler = async event => {
  let success = 0;
  let failure = 0;

  const streamerToLeaderboardEntryMap = new Map();

  let recordOutput = [];
  event.records.forEach(record => {
    try {
      const recordData = Buffer.from(record.data, "base64");

      let { streamer_id, user_id, PUG_COUNT } = JSON.parse(
        recordData.toString("ascii")
      );

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

  for (const [streamer_id, userToScoreMap] of streamerToLeaderboardEntryMap) {
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

      let { leaderboard } = previousLeaderboardForStreamer;

      userToScoreMap.forEach((PUG_COUNT, user_id) => {
        const previousLeaderboardPlayerEntryIndex = previousLeaderboardForStreamer.leaderboard.findIndex(
          ({ player }) => user_id === player
        );

        if (previousLeaderboardPlayerEntryIndex > -1) {
          // player already exists in leaderboard so bump score from latest window
          leaderboard[previousLeaderboardPlayerEntryIndex].score += PUG_COUNT;
        } else {
          // player previously did not exist in leaderboard
          leaderboard.push({
            player: user_id,
            score: PUG_COUNT
          });
        }
      });

      const updatedScoresForStreamer = {
        streamer_id,
        leaderboard: leaderboard.sort(
          ({ score: aScore }, { score: bScore }) => bScore - aScore
        )
      };

      updatedScoresForStreamerPutParams = {
        TableName: process.env.LEADERS_TABLE,
        Item: AWS.DynamoDB.Converter.marshall(updatedScoresForStreamer)
      };

      await ddb.putItem(updatedScoresForStreamerPutParams).promise();
    } catch (error) {
      console.error(`Failed to update table.`, error);
    }
  }

  console.log(
    `Successfully delivered records ${success}, Failed delivered records ${failure}.`
  );

  return {
    records: recordOutput
  };
};
