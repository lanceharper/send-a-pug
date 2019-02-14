open ApolloInMemoryCache;

let inMemoryCache = createInMemoryCache();

let headerContextLink =
  ApolloLinks.createContextLink(() =>
    {
      "headers": {
        "x-api-key": "da2-xcepzyehbfemxg5qdjbyf6qsl4",
      },
    }
  );

let httpLink =
  ApolloLinks.createHttpLink(
    ~uri=
      "https://iej7kez7cjhr7emzp5kn76wt24.appsync-api.us-west-2.amazonaws.com/graphql",
    (),
  );

let instance =
  ReasonApollo.createApolloClient(
    ~link=ApolloLinks.from([|headerContextLink, httpLink|]),
    ~cache=inMemoryCache,
    (),
  );