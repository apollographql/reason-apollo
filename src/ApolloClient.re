open ReasonApolloTypes;

module type ApolloClientConfig = {type variables};

module Get = (ApolloClientConfig: ApolloClientConfig) => {
  type queryObj = {. "query": queryString, "variables": ApolloClientConfig.variables};

  type generatedApolloClient = {. "query": [@bs.meth] (queryObj => string)};

  type clientOptions = {. "cache": unit, "link": unit};

  type linkOptions = {. "uri": string};

  [@bs.module "apollo-client"] [@bs.new]
  external apolloClient : clientOptions => generatedApolloClient =
    "ApolloClient";

  [@bs.module "apollo-link-http"] [@bs.new] external httpLink : linkOptions => 'a = "HttpLink";

  [@bs.module "apollo-cache-inmemory"] [@bs.new] external inMemoryCache : unit => 'a =
    "InMemoryCache";
}
