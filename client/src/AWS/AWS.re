module AppSyncConfig = {
  type config = {
    .
    "graphqlEndpoint": string,
    "region": string,
    "authenticationType": string,
    "jwtToken": string,
  };
  let config = {
    "graphqlEndpoint": "https://5omqgexqabbgtkqwfwanlngtr4.appsync-api.us-west-2.amazonaws.com/graphql",
    "region": "us-west-2",
    "authenticationType": "AMAZON_COGNITO_USER_POOLS",
  };
};

module AppSync = {
  [@bs.module "aws-appsync"] [@bs.new]
  external makeAWSAppSyncClient:
    {
      .
      "url": Js.Nullable.t(string),
      "region": Js.Nullable.t(string),
      "auth":
        Js.Nullable.t({
          .
          "type": string,
          "jwtToken": string,
        }),
    } =>
    'a =
    "AWSAppSyncClient";
  let createAWSAppSyncClient = (~url=?, ~region=?, ~auth=?, ()) => {
    let appSyncClientOptions = {
      "url": Js.Nullable.fromOption(url),
      "region": Js.Nullable.fromOption(region),
      "auth": Js.Nullable.fromOption(auth),
    };
    makeAWSAppSyncClient(appSyncClientOptions);
  };
};