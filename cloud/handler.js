const AWS = require("aws-sdk");
const jwt = require("jsonwebtoken");

const verifyAndDecode = auth => {
  const bearerPrefix = "Bearer ";
  if (!auth.startsWith(bearerPrefix))
    return { err: "Invalid authorization header" };
  try {
    const token = auth.substring(bearerPrefix.length);
    const secret = process.env.secret;
    return jwt.verify(token, Buffer.from(secret, "base64"), {
      algorithms: ["HS256"]
    });
  } catch (err) {
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
      body: JSON.stringify({ foo: "bar" }, null, 2),
      headers
    };
  };

  console.log("event", event);
  //const payload = verifyAndDecode(event.headers.Authorization);

  //if (payload.err) return response(401, JSON.stringify(payload));

  return response(200, event.hello);
};
