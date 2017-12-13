module type CreationConfig = {let uri: string;};

module type ClientConfig = {type responseType; type variables;};

module Create = (CreationConfig: CreationConfig) => {
  let httpLinkOptions: ApolloClient.linkOptions = {"uri": CreationConfig.uri};
  let apolloClientOptions: ApolloClient.clientOptions = {
    "cache": ApolloClient.inMemoryCache(),
    "link": ApolloClient.httpLink(httpLinkOptions)
  };
  let apolloClient = ApolloClient.apolloClient(apolloClientOptions);

  module ReasonApolloQuery.Query;
};