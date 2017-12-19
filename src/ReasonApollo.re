module type CreationConfig = {let uri: string;};

module Create = (CreationConfig: CreationConfig) => {
  let httpLinkOptions: ApolloClient.linkOptions = {"uri": CreationConfig.uri};
  let apolloClientOptions: ApolloClient.clientOptions = {
    "cache": ApolloClient.inMemoryCache(),
    "link": ApolloClient.httpLink(httpLinkOptions)
  };
  let apolloClient = ApolloClient.apolloClient(apolloClientOptions);

  module Query = ReasonApolloQuery.QueryFactory({ let apolloClient = apolloClient;});
  module Mutation = ReasonApolloMutation.MutationFactory({let apolloClient = apolloClient;});
};