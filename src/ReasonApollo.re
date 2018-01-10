let createApolloClient =
    (
      ~cache,
      ~link,
      ~ssrMode=?,
      ~ssrForceFetchDelay=?,
      ~connectToDevTools=?,
      ~queryDeduplication=?,
      ()
    ) => {
  let apolloClientOptions = {
    "link": link,
    "cache": cache,
    "ssrMode": Js.Nullable.from_opt(ssrMode),
    "ssrForceFetchDelay": Js.Nullable.from_opt(ssrForceFetchDelay),
    "connectToDevTools": Js.Nullable.from_opt(connectToDevTools),
    "queryDeduplication": Js.Nullable.from_opt(queryDeduplication)
  };
  ApolloClient.apolloClient(apolloClientOptions)
};

module type ApolloClientConfig = {let createApolloClient: ApolloClient.generatedApolloClient;};

module CreateClient = (Config: ApolloClientConfig) => {
  let apolloClient = Config.createApolloClient;
  module Query =
    ReasonApolloQuery.QueryFactory(
      {
        let apolloClient = apolloClient;
      }
    );
  module Mutation =
    ReasonApolloMutation.MutationFactory(
      {
        let apolloClient = apolloClient;
      }
    );
};