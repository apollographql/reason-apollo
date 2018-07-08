open ApolloClient;

  /*
  * Expose a createApolloClient function that has to be passed to the ApolloProvider
  */
  let createApolloClient = (
    ~link,
    ~cache,
    ~ssrMode=?,
    ~ssrForceFetchDelay=?,
    ~connectToDevTools=?,
    ~queryDeduplication=?,
    ()
  ) => {
    createApolloClientJS(
      apolloClientObjectParam(
        ~link,
        ~cache,
        ~ssrMode=?ssrMode,
        ~ssrForceFetchDelay=?ssrForceFetchDelay,
        ~connectToDevTools=?connectToDevTools,
        ~queryDeduplication=?queryDeduplication
      )
    );
  };

  /*
  * Expose a module to perform "query" operations for the given client
  */
  module CreateQuery = (Config:ReasonApolloTypes.Config) =>
    ReasonApolloQuery.Get(Config);
    
  /*
  * Expose a module to perform "mutation" operations for the given client
  */
  module CreateMutation = (Config:ReasonApolloTypes.Config) =>
    ReasonApolloMutation.MutationFactory(Config);
  /*
   * Expose a module to perform "subscription" operations for the given client 
   */
  module CreateSubscription = (Config:ReasonApolloTypes.Config) => 
    ReasonApolloSubscription.Get(Config);

  module Provider = ApolloProvider;

  module Consumer = ApolloConsumer;
