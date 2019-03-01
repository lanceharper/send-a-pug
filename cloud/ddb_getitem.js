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

var js = {
  streamer_id: "puglugger",
  leaderboard: [
    {
      player: "puglugger",
      score: 100
    },
    {
      player: "pugloogr",
      score: 300
    },
    {
      player: "puglaagr",
      score: 500
    }
  ]
};

var putParams = {
  TableName: "dev-jerk-leaders",
  Item: AWS.DynamoDB.Converter.marshall(js)
};

ddb.getItem(params, function(err, data) {
  if (err) {
    console.log("Error", err);
  } else {
    console.log("Success", AWS.DynamoDB.Converter.unmarshall(data.Item));
  }
});

ddb.putItem(putParams, function(err, data) {
  if (err) {
    console.log("Error", err);
  } else {
    console.log("Success", data);
  }
});
