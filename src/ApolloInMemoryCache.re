open ReasonApolloTypes;

module type InMemoryCacheConfig = {
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

module CreateInMemoryCache = (Config: InMemoryCacheConfig) => {
  /* Define the type of the object to pas to the InMemoryCache constructor */
  type inMemoryCacheObject = Js.Nullable.t({. "dataIdFromObject": Config.dataObject => string});
  /* Bind the InMemoryCache class */
  [@bs.module "apollo-cache-inmemory"] [@bs.new]
  external inMemoryCache : inMemoryCacheObject => apolloCache =
    "InMemoryCache";
  /* Instanciate a new cache object */
  let cache = inMemoryCache(Config.inMemoryCacheObject);
};