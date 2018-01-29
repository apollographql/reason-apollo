open ReasonApolloTypes;

/**
 * Used on the client to rehydrate the cache using the initial data passed from the server
 * - e.g. window.__APOLLO_STATE__
 */
type restoreData;

/**
 * Define the data to pass to the restore method that'll be used used to rehydrate client.
 * If you don't want to pass any data, simply return `Js_null_undefined.undefined`.
 */
type inMemoryCacheRestoreData = Js.Nullable.t(restoreData);

/**
 * CreateInMemoryCache
 * https://github.com/apollographql/apollo-client/tree/master/packages/apollo-cache-inmemory
 */
/* Bind the InMemoryCache class */
[@bs.module "apollo-cache-inmemory"]
[@bs.new]
external apolloInMemoryCache : 'a => apolloCache = "InMemoryCache";

/* Bind the restore method */
[@bs.send.pipe : 't]
external restore : inMemoryCacheRestoreData => apolloCache = "restore";

/* Fragment matcher */
type fragmentMatcher;

[@bs.module "apollo-cache-inmemory"] [@bs.new]
external introspectionFragmentMatcher : 'a => fragmentMatcher =
  "IntrospectionFragmentMatcher";

let createIntrospectionFragmentMatcher = (~data) =>
  introspectionFragmentMatcher({"introspectionQueryResultData": data});

/* Instantiate a new cache object */
let createInMemoryCache = (~dataIdFromObject=?, ~fragmentMatcher=?, ()) =>
  switch fragmentMatcher {
  | Some(fragmentMatcher) =>
    apolloInMemoryCache({
      "dataIdFromObject": Js.Nullable.from_opt(dataIdFromObject),
      "fragmentMatcher": Js.Nullable.return(fragmentMatcher)
    })
  | None =>
    apolloInMemoryCache({
      "dataIdFromObject": Js.Nullable.from_opt(dataIdFromObject)
    })
  };
