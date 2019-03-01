console.log("Loading function");

const AWS = require("aws-sdk");

// AWS.config.update({ region: "us-west-2" });

const ddb = new AWS.DynamoDB({ apiVersion: "2012-08-10" });

exports.handler = async event => {
  let success = 0;
  let failure = 0;

  const streamerToLeaderboardEntryMap = new Map();

  const output = event.records.forEach(record => {
    try {
      const recordData = Buffer.from(record.data, "base64");

      const { streamer_id, user_id, PUG_COUNT } = recordData.toString("ascii");

      if (!streamerToLeaderboardEntryMap.has(streamer_id)) {
        streamerToLeaderboardEntryMap.set(
          streamer_id,
          new Map([user_id, PUG_COUNT])
        );
      } else {
        streamerToLeaderboardEntryMap.get(streamer_id).set(user_id, PUG_COUNT);
      }
      success++;
      return {
        recordId: record.recordId,
        result: "Ok"
      };
    } catch (err) {
      failure++;
      console.error(err);
      return {
        recordId: record.recordId,
        result: "DeliveryFailed"
      };
    }
  });

  await streamerToLeaderboardEntryMap.forEach(
    async (userToScoreMap, streamer_id) => {
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

      const {
        player,
        score: previousScore
      } = AWS.DynamoDB.Converter.unmarshall(previousLeaderboardDDBItem);

      // If there are previous scores for a streamer's user,
      // increase their total score by the amount returned over the last time window
      const updatedScoresForStreamer = {
        streamer_id,
        leaderboard: userToScoreMap.forEach((PUG_COUNT, user_id) => ({
          player: user_id,
          score: player ? previousScore + PUG_COUNT : PUG_COUNT
        }))
      };

      updatedScoresForStreamerPutParams = {
        TableName: process.env.LEADERS_TABLE,
        Item: AWS.DynamoDB.Converter.marshall(updatedScoresForStreamer)
      };

      await ddb.putItem(updatedScoresForStreamerPutParams).promise();
    }
  );

  console.log(
    `Successfully delivered records ${success}, Failed delivered records ${failure}.`
  );

  return {
    records: output
  };
};
