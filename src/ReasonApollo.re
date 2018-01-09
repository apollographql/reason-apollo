let apolloClient = Apollo.createApolloClient;

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