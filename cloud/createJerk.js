const AWS = require("aws-sdk");

const kinesis = new AWS.Kinesis();

module.exports.handler = async event => {
  const { from: user_id, to: streamer_id } = event.arguments;
  recordParams = {
    Data: JSON.stringify({ streamer_id, user_id }),
    PartitionKey: streamer_id,
    StreamName: process.env.JERK_STREAM
  };

  await kinesis.putRecord(recordParams).promise();

  return { ...event.arguments, sentAt: new Date().toISOString() };
};
