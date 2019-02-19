const AWS = require("aws-sdk");
const jwt = require("jsonwebtoken");

const verifyAndDecode = twitchToken => {
  try {
    const secret = process.env.JERK_TWITCH_SECRET;
    return jwt.verify(twitchToken, Buffer.from(secret, "base64"), {
      algorithms: ["HS256"]
    });
  } catch (err) {
    console.log("err", err);
    return { err: "Invalid JWT" };
  }
};

module.exports.hello = async (event, context) => {
  const response = (statusCode, body) => {
    const headers = {
      "Access-Control-Allow-Origin": "*",
      "Access-Control-Allow-Credentials": true
    };

    return {
      statusCode,
      body: JSON.stringify(body, null, 2),
      headers
    };
  };

  const payload = verifyAndDecode(JSON.parse(event.body).twitchToken);

  if (payload.err) return response(401, JSON.stringify(payload));

  return response(200, payload);
};
