const AWS = require("aws-sdk");
const jwt = require("jsonwebtoken");
const fetch = require("node-fetch");

const cognitoIdentityServiceProvider = new AWS.CognitoIdentityServiceProvider();

const newUserPassword = "f00BarBaz";
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

const getCognitoUserFromTwitchUser = async twitchUser => {
  let params = {
    UserPoolId: process.env.USER_POOL_ID,
    Username: twitchUser["_id"]
  };

  const createdCognitoUser = await cognitoIdentityServiceProvider
    .adminGetUser(params)
    .promise();
  return createdCognitoUser;
};

const initiateCognitoAuth = async cognitoUser => {
  let params = {
    AuthFlow: "ADMIN_NO_SRP_AUTH",
    UserPoolId: process.env.USER_POOL_ID,
    ClientId: process.env.USER_POOL_CLIENT_ID,
    AuthParameters: {
      USERNAME: cognitoUser.Username,
      PASSWORD: newUserPassword
    }
  };

  try {
    const createdInitiatedAuthUserResponse = await cognitoIdentityServiceProvider
      .adminInitiateAuth(params)
      .promise();
    return createdInitiatedAuthUserResponse;
  } catch (createdInitiatedAuthError) {
    console.error("createdInitiatedAuthError", createdInitiatedAuthError);
    return cognitoUser;
  }
};

const respondToAuthChallenge = async (cognitoUser, initiatedAuthResponse) => {
  let params = {
    ClientId: process.env.USER_POOL_CLIENT_ID,
    ChallengeName: "NEW_PASSWORD_REQUIRED",
    ChallengeResponses: {
      USERNAME: cognitoUser.Username,
      NEW_PASSWORD: newUserPassword
    },
    Session: initiatedAuthResponse.Session,
    UserPoolId: process.env.USER_POOL_ID
  };

  const adminRespondToAuthChallenge = await cognitoIdentityServiceProvider
    .adminRespondToAuthChallenge(params)
    .promise();

  return adminRespondToAuthChallenge;
};

const createCognitoUserFromTwitchUser = async twitchUser => {
  let params = {
    UserPoolId: process.env.USER_POOL_ID,
    Username: twitchUser["_id"],
    MessageAction: "SUPPRESS", // Do not send welcome email
    TemporaryPassword: newUserPassword,
    UserAttributes: [
      { Name: "preferred_username", Value: twitchUser.display_name }
    ]
  };

  const createdCognitoUser = await cognitoIdentityServiceProvider
    .adminCreateUser(params)
    .promise();
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

  try {
    const payload = verifyAndDecode(JSON.parse(event.body).twitchToken);

    const twitchUser = await getTwitchUser(payload.user_id);

    let cognitoUser = null;

    try {
      cognitoUser = await getCognitoUserFromTwitchUser(twitchUser);
    } catch ({ code }) {
      if (code === "UserNotFoundException") {
        cognitoUser = await createCognitoUserFromTwitchUser(twitchUser);
      } else {
        return response(500, JSON.stringify(error));
      }
    }

    if (cognitoUser.User) {
      cognitoUser = cognitoUser.User;
    }

    if (cognitoUser.UserStatus === "FORCE_CHANGE_PASSWORD") {
      const cognitoAuthResponse = await initiateCognitoAuth(cognitoUser);
      const challengeResponse = await respondToAuthChallenge(
        cognitoUser,
        cognitoAuthResponse
      );
      return response(200, challengeResponse.AuthenticationResult);
    } else if (cognitoUser.UserStatus === "CONFIRMED") {
      const cognitoAuthResponse = await initiateCognitoAuth(cognitoUser);
      return response(200, cognitoAuthResponse.AuthenticationResult);
    }

    return response(500, JSON.stringify(payload));
  } catch (ohNoError) {
    return response(500, JSON.stringify(ohNoError));
  }
};
