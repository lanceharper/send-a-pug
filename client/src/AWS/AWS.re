module AppSyncConfig = {
  type config = {
    .
    "graphqlEndpoint": string,
    "region": string,
    "authenticationType": string,
    "apiKey": string,
  };
  let config = {
    "graphqlEndpoint": "https://iej7kez7cjhr7emzp5kn76wt24.appsync-api.us-west-2.amazonaws.com/graphql",
    "region": "us-west-2",
    "authenticationType": "API_KEY",
    "apiKey": "da2-xcepzyehbfemxg5qdjbyf6qsl4",
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
          "apiKey": string,
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