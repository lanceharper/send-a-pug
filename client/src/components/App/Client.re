open AWS.AppSyncConfig;

let instance = jwtToken =>
  AWS.AppSync.createAWSAppSyncClient(
    ~url=config##graphqlEndpoint,
    ~region=config##region,
    ~auth={"type": config##authenticationType, "jwtToken": jwtToken},
    (),
  );