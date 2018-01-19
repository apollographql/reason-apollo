open ReasonApolloTypes;

/**
 * Describe your data object shape. Usually you have following keys:
 * - "__typename": string
 * - "id": string
 */
type dataObject;

/**
 * Define the object to pass to the InMemoryCache constructor.
 * If you don't want to pass any object, simply return `Js_null_undefined.undefined`.
 */
let inMemoryCacheObject:
  /* TODO: define missing fields */
  Js.Nullable.t({. "dataIdFromObject": dataObject => string});

/**
 * CreateInMemoryCache
 * https://github.com/apollographql/apollo-client/tree/master/packages/apollo-cache-inmemory
 */

/* Bind the InMemoryCache class */
[@bs.module "apollo-cache-inmemory"] [@bs.new]
external createInMemoryCache : inMemoryCacheObject => apolloCache =
  "InMemoryCache";

/* Instantiate a new cache object */
let createInMemoryCache = (inMemoryCacheObject) => apolloInMemoryCache(inMemoryCacheObject);