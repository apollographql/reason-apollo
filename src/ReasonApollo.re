open ReasonApolloTypes;

/**
 * CreateInMemoryCache
 * https://github.com/apollographql/apollo-client/tree/master/packages/apollo-cache-inmemory
 */
module type ApolloInMemoryCacheConfig = {
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
};

module CreateInMemoryCache = (Config: ApolloInMemoryCacheConfig) => {
  /* Define the type of the object to pas to the InMemoryCache constructor */
  type inMemoryCacheObject =
    Js.Nullable.t({. "dataIdFromObject": Config.dataObject => string});
  /* Bind the InMemoryCache class */
  [@bs.module "apollo-cache-inmemory"] [@bs.new]
  external createInMemoryCache : inMemoryCacheObject => apolloCache =
    "InMemoryCache";
  /* Instanciate a new cache object */
  let cache = createInMemoryCache(Config.inMemoryCacheObject);
};

/**
 * CreateClient
 * https://github.com/apollographql/apollo-client
 */
module type ApolloClientConfig = {
  /**
   * An array of links, each one constructed with one of the available
   * modules in ApolloLinks
   */
  let links: array(apolloLink);
  /**
   * A cache object, usually an instance of InMemoryCache
   */
  let cache: apolloCache;
};

module CreateClient = (Config: ApolloClientConfig) => {
  /* Bind the ApolloClient class */
  [@bs.module "apollo-client"] [@bs.new]
  external createApolloClient :
    clientOptions => ApolloClient.generatedApolloClient =
    "ApolloClient";
  /* Bind the method `from`, used to compose links together */
  [@bs.module "apollo-link"]
  external createApolloLinkFrom : array(apolloLink) => apolloLink = "from";
  /*Instanciate a new client object */
  let apolloClient =
    createApolloClient({
      "cache": Config.cache,
      "link": createApolloLinkFrom(Config.links)
    });
  /**
   * Expose a module to perform "query" operations for the given client
   */
  module Query =
    ReasonApolloQuery.QueryFactory(
      {
        let apolloClient = apolloClient;
      }
    );
  /**
   * Expose a module to perform "mutation" operations for the given client
   */
  module Mutation =
    ReasonApolloMutation.MutationFactory(
      {
        let apolloClient = apolloClient;
      }
    );
};