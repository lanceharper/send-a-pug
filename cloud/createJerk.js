const AWS = require("aws-sdk");

const kinesis = new AWS.Kinesis();

module.exports.handler = async (event, context) => {
  recordParams = {
    Data: JSON.stringify({ foo: "bar" }),
    PartitionKey: "foo",
    StreamName: process.env.JERK_STREAM
  };

  const recordResponse = await kinesis.putRecord(recordParams).promise();

  return {
    statusCode: 200,
    body: JSON.stringify(recordResponse, null, 2)
  };
};
