let createClient =
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