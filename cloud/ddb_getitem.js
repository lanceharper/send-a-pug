var AWS = require("aws-sdk");
// Set the region
AWS.config.update({ region: "us-west-2" });

// Create the DynamoDB service object
var ddb = new AWS.DynamoDB({ apiVersion: "2012-08-10" });
var documentClient = new AWS.DynamoDB.DocumentClient({ region: "us-west-2" });

var params = {
  TableName: "dev-jerk-leaders",
  Key: {
    streamer_id: { S: "puglugger" }
  },
  ProjectionExpression: "streamer_id, leaderboard"
};

var putParams = {
  TableName: "dev-jerk-leaders",
  Item: {
    streamer_id: {
      S: "puglugger"
    },
    leaderboard: {
      L: [
        {
          M: {
            player: { S: "puglugger" },
            score: { N: "100" }
          }
        },
        {
          M: {
            player: { S: "pugloogr" },
            score: { N: "200" }
          }
        }
      ]
    }
  }
};

ddb.getItem(params, function(err, data) {
  if (err) {
    console.log("Error", err);
  } else {
    console.log("Success", data.Item);
  }
});

ddb.putItem(putParams, function(err, data) {
  if (err) {
    console.log("Error", err);
  } else {
    console.log("Success", data);
  }
});
