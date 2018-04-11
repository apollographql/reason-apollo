open ApolloClient;

[@bs.module "apollo-client"] [@bs.new]
external createApolloClient : clientOptions => generatedApolloClient =
  "ApolloClient";  
  /*
  * Expose a module to perform "query" operations for the given client
  */
  module CreateQuery = (Config:ReasonApolloTypes.Config) =>
    ReasonApolloQuery.Get(Config);
    
  /*
  * Expose a module to perform "mutation" operations for the given client
  */
  module CreateMutation =
    ReasonApolloMutation.MutationFactory();

  module Provider = ApolloProvider;

  module Consumer = ApolloConsumer;