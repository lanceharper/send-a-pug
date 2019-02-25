const AWS = require("aws-sdk");

const kinesis = new AWS.Kinesis();

module.exports.handler = async (event, context) => {
  console.log("event", event);

  recordParams = {
    Data: JSON.stringify({ foo: "bar" }),
    PartitionKey: "foo",
    StreamName: process.env.JERK_STREAM
  };

  // const recordResponse = await kinesis.putRecord(recordParams).promise();

  return { ...event.arguments, sentAt: "now" };
};
