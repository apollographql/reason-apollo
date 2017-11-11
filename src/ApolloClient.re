open ReasonApolloTypes;

type queryObj = {. "query": queryString};

type generatedApolloClient = {. "query": [@bs.meth] (queryObj => string)};

type clientOptions = {. "cache": unit, "link": unit};

type linkOptions = {. "uri": string};

[@bs.module "apollo-client"] [@bs.new]
external apolloClient : clientOptions => generatedApolloClient =
  "ApolloClient";

[@bs.module "apollo-link-http"] [@bs.new] external httpLink : linkOptions => 'a = "HttpLink";

[@bs.module "apollo-cache-inmemory"] [@bs.new] external inMemoryCache : unit => 'a =
  "InMemoryCache";