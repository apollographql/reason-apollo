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
  ApolloClient.createApolloClient(apolloClientOptions)
};

module type ApolloClientConfig = {let apolloClient: ApolloClient.generatedApolloClient;};


module type Client = {
  let apolloClient: ApolloClient.generatedApolloClient;

  module Query: ReasonApolloQuery.Query;
  module Mutation: ReasonApolloMutation.Mutation;
};

module CreateClient = (Config: ApolloClientConfig) : Client => {
  let apolloClient = Config.apolloClient;

  /*
  * Expose a module to perform "query" operations for the given client
  */
  module Query =
    ReasonApolloQuery.QueryFactory(
      {
        let apolloClient = apolloClient;
      }
    );
    
  /*
  * Expose a module to perform "mutation" operations for the given client
  */
  module Mutation =
    ReasonApolloMutation.MutationFactory(
      {
        let apolloClient = apolloClient;
      }
    );
};
