open ApolloClient;

let mapPolicies = (options, key, policies) => {
  open ReasonApolloTypes;

  let {errorPolicy, fetchPolicy} = policies;

  Js.Dict.set(
    options,
    key,
    Js.Dict.fromList([
      ("errorPolicy", errorPolicyToJs(errorPolicy)),
      ("fetchPolicy", fetchPolicyToJs(fetchPolicy)),
    ]),
  );
};

let makeDefaultOptions = (~mutate=?, ~query=?, ~watchQuery=?, ()) => {
  let opts = Js.Dict.empty();

  [("mutate", mutate), ("query", query), ("watchQuery", watchQuery)]
  ->Belt.List.forEach(((key, policy)) =>
      Belt.Option.map(policy, mapPolicies(opts, key))
    );

  opts;
};

/*
 * Expose a createApolloClient function that has to be passed to the ApolloProvider
 */
let createApolloClient =
    (
      ~link,
      ~cache,
      ~ssrMode=?,
      ~ssrForceFetchDelay=?,
      ~connectToDevTools=?,
      ~queryDeduplication=?,
      ~defaultOptions=?,
      (),
    ) =>
  createApolloClientJS({
    link,
    cache,
    ssrMode,
    ssrForceFetchDelay,
    connectToDevTools,
    queryDeduplication,
    defaultOptions,
  });
// let createApolloClient =
//     (
//       ~link,
//       ~cache,
//       ~ssrMode=?,
//       ~ssrForceFetchDelay=?,
//       ~connectToDevTools=?,
//       ~queryDeduplication=?,
//       (),
//     ) => {
//   createApolloClientJS(
//     apolloClientObjectParam(
//       ~link,
//       ~cache,
//       ~ssrMode?,
//       ~ssrForceFetchDelay?,
//       ~connectToDevTools?,
//       ~queryDeduplication?,
//     ),
//   );
// };

/*
 * Expose a module to perform "query" operations for the given client
 */
module CreateQuery = (Config: ReasonApolloTypes.Config) =>
  ReasonApolloQuery.Make(Config);

/*
 * Expose a module to perform "mutation" operations for the given client
 */
module CreateMutation = (Config: ReasonApolloTypes.Config) =>
  ReasonApolloMutation.Make(Config);
/*
 * Expose a module to perform "subscription" operations for the given client
 */
module CreateSubscription = (Config: ReasonApolloTypes.Config) =>
  ReasonApolloSubscription.Make(Config);

module Provider = ApolloProvider;

module Consumer = ApolloConsumer;
