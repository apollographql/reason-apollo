let createClient =
    (
      ~cache,
      ~link,
      ~ssrMode=false,
      ~ssrForceFetchDelay=0,
      ~connectToDevTools=Js.Nullable.undefined,
      ~queryDeduplication=true,
      ()
    ) => {
  let apolloClientOptions = {
    "link": link,
    "cache": cache,
    "ssrMode": Js.Boolean.to_js_boolean(ssrMode),
    "ssrForceFetchDelay": ssrForceFetchDelay,
    "connectToDevTools": connectToDevTools,
    "queryDeduplication": Js.Boolean.to_js_boolean(queryDeduplication)
  };
  ApolloClient.apolloClient(apolloClientOptions)
};