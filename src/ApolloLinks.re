open ReasonApolloTypes;

/* Bind the method `from`, used to compose links together */
[@bs.module "apollo-link"] external from : array(apolloLink) => apolloLink = "from";
/* Bind the HttpLink class */
[@bs.module "apollo-link-http"] [@bs.new] external createHttpLink : ApolloClient.linkOptions => apolloLink = "HttpLink";
/* Bind the setContext method */
[@bs.module "apollo-link-context"] external apolloLinkSetContext : (unit => Js.t({..})) => apolloLink = "setContext";
/* Bind the onError method */
[@bs.module "apollo-link-error"] external apolloLinkOnError : (apolloLinkErrorResponse => unit) => apolloLink = "onError";

/**
 * CreateHttpLink
 * https://github.com/apollographql/apollo-link/tree/master/packages/apollo-link-http
 */
let createHttpLink = (
~uri,
~includeExtensions=?,
~fetch=?,
~headers=?,
~credentials=?,
~fetchOptions=?,
()
) => {
    createHttpLink({
      "uri": uri,
      "includeExtensions": Js.Nullable.from_opt(includeExtensions),
      "fetch": Js.Nullable.from_opt(fetch),
      "headers": Js.Nullable.from_opt(headers),
      "credentials": Js.Nullable.from_opt(credentials),
      "fetchOptions": Js.Nullable.from_opt(fetchOptions)
    });
};

/**
 * CreateContextLink
 * https://github.com/apollographql/apollo-link/tree/master/packages/apollo-link-context
 */
let createContextLink = (contextHandler) => {
  /* Instanciate a new context link object */
  apolloLinkSetContext(contextHandler);
};

/**
 * CreateErrorLink
 * https://github.com/apollographql/apollo-link/tree/master/packages/apollo-link-error
 */
let createErrorLink = (errorHandler) => {
  /* Instanciate a new error link object */
  apolloLinkOnError(errorHandler);
};