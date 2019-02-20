const AWS = require("aws-sdk");
const jwt = require("jsonwebtoken");
const fetch = require("node-fetch");
const uuidv1 = require("uuid/v1");

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

const createCognitoUserFromTwitchUser = async twitchUser => {
  let cognitoIdentityServiceProvider = new AWS.CognitoIdentityServiceProvider({
    apiVersion: "2016-04-18"
  });

  let params = {
    UserPoolId: process.env.USER_POOL_ID,
    Username: uuidv1(),
    MessageAction: "SUPPRESS", // Do not send welcome email
    TemporaryPassword: "f00bar",
    UserAttributes: [
      { Name: "preferred_username", Value: twitchUser.display_name }
    ]
  };

  const createdCognitoUser = await cognitoIdentityServiceProvider
    .adminCreateUser(params)
    .promise();
  console.log("createdCognitoUser", createdCognitoUser);
  return createdCognitoUser;
};

const getTwitchUser = async userId => {
  const userResponse = await fetch(
    `https://api.twitch.tv/kraken/users/${userId}`,
    {
      method: "GET",
      headers: {
        "Client-ID": process.env.TWITCH_CLIENT_ID,
        Accept: "application/vnd.twitchtv.v5+json"
      }
    }
  );
  return await userResponse.json();
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

  const twitchUser = await getTwitchUser(payload.user_id);

  const cognitoUser = await createCognitoUserFromTwitchUser(twitchUser);

  console.log("twitchUser", twitchUser);

  if (payload.err) return response(401, JSON.stringify(payload));

  return response(200, cognitoUser);
};
