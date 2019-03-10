//const AWS = require("aws-sdk-mock");
const aggregateJerk = require("../aggregateJerk");

const pugluggerStringifiedRecord = JSON.stringify({
  streamer_id: "233205905",
  user_id: "puglugger",
  PUG_COUNT: 2
});

const pugloogrStringifiedRecord = JSON.stringify({
  streamer_id: "233205905",
  user_id: "pugloogr",
  PUG_COUNT: 5
});

const pugleegrStringifiedRecord = JSON.stringify({
  streamer_id: "233205905",
  user_id: "pugleegr",
  PUG_COUNT: 50
});

const pugleegrOtherStreamerStringifiedRecord = JSON.stringify({
  streamer_id: "233205906",
  user_id: "pugleegr",
  PUG_COUNT: 50
});

const kinesisEvent = {
  records: [
    {
      recordId: 123,
      data: Buffer.from(pugluggerStringifiedRecord).toString("base64")
    },
    {
      recordId: 124,
      data: Buffer.from(pugloogrStringifiedRecord).toString("base64")
    },
    {
      recordId: 125,
      data: Buffer.from(pugleegrStringifiedRecord).toString("base64")
    },
    {
      recordId: 126,
      data: Buffer.from(pugleegrOtherStreamerStringifiedRecord).toString(
        "base64"
      )
    }
  ]
};

aggregateJerk.handler(kinesisEvent).then(aggregateJerkResponse => {
  console.log("aggregateJerkResponse", aggregateJerkResponse);
});
