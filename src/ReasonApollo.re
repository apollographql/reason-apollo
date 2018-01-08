open ReasonApolloTypes;

/**
   * CreateClient
   * https://github.com/apollographql/apollo-client
   */
module type CreationConfig = {let createApolloClient: ApolloClient.generatedApolloClient;};

module Create = (CreationConfig: CreationConfig) => {
  let apolloClient = CreationConfig.createApolloClient;
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