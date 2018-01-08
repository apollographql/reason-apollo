open ReasonApolloTypes;

/**
 * A convenience method to cast a generic type to a record object.
 */
external asJsObject : 'a => Js.t({..}) = "%identity";

/**
 * CreateHttpLink
 * https://github.com/apollographql/apollo-link/tree/master/packages/apollo-link-http
 */
module type HttpLinkConfig = {
  /**
   * The endpoint where the GraphQL server is running.
   * https://github.com/apollographql/apollo-link/tree/master/packages/apollo-link-http#options
   * TODO: support other options
   */
  let uri: string;
};

module CreateHttpLink = (Config: HttpLinkConfig) => {
  /* Bind the HttpLink class */
  [@bs.module "apollo-link-http"] [@bs.new]
  external createHttpLink : ApolloClient.linkOptions => apolloLink =
    "HttpLink";
  /* Instanciate a new http link object */
  let link = createHttpLink({"uri": Config.uri});
};

/**
 * CreateContextLink
 * https://github.com/apollographql/apollo-link/tree/master/packages/apollo-link-context
 */
module type ContextLinkConfig = {
  /**
   * Define the callback function to set the new context of a request.
   * The function returns an object record (use the utility function `asJsObject`)
   * to cast your custom object to this generic type.
   * TODO: support passing the arguments of the function.
   */
  let contextHandler: unit => Js.t({..});
};

module CreateContextLink = (Config: ContextLinkConfig) => {
  /* Bind the setContext method */
  [@bs.module "apollo-link-context"]
  external apolloLinkSetContext : (unit => Js.t({..})) => apolloLink =
    "setContext";
  /* Instanciate a new context link object */
  let link = apolloLinkSetContext(Config.contextHandler);
};

/**
 * CreateErrorLink
 * https://github.com/apollographql/apollo-link/tree/master/packages/apollo-link-error
 */
type networkError = {. "statusCode": int};

/* TODO: define missing keys */
type apolloLinkErrorResponse = {. "networkError": option(networkError)};

module type ErrorLinkConfig = {
  /**
   * Define the callback function that gets called in the event of an error.
   * This function is called with an object containing the keys defined here:
   * https://github.com/apollographql/apollo-link/tree/master/packages/apollo-link-error#callback
   * NOTE: at the moment only `networkError` field is supported
   */
  let errorHandler: apolloLinkErrorResponse => unit;
};

module CreateErrorLink = (Config: ErrorLinkConfig) => {
  /* Bind the onError method */
  [@bs.module "apollo-link-error"]
  external apolloLinkOnError : (apolloLinkErrorResponse => unit) => apolloLink =
    "onError";
  /* Instanciate a new error link object */
  let link = apolloLinkOnError(Config.errorHandler);
};