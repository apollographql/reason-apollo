open ReasonApolloTypes;

type queryObj = {. "query": string, "variables": Js.Json.t };

type generatedApolloClient = {. "query": [@bs.meth] (queryObj => string)};

type clientOptions = {
  .
  "link": apolloLink,
  "cache": apolloCache,
  "ssrMode": Js.Nullable.t(Js.boolean),
  "ssrForceFetchDelay": Js.Nullable.t(int),
  "connectToDevTools": Js.Nullable.t(Js.boolean),
  "queryDeduplication": Js.Nullable.t(Js.boolean)
};

type linkOptions = {. "uri": string};

[@bs.module "apollo-client"] [@bs.new]
external createApolloClient : clientOptions => generatedApolloClient =
  "ApolloClient";

module type ApolloClientCast = {type variables;};

/* Cast the apolloClient, with the known variable type when called */
module Cast = (ApolloClientCast: ApolloClientCast) => {
  type queryObj = {. "query": queryString, "variables": ApolloClientCast.variables};
  type mutationObj = {. "mutation": queryString, "variables": ApolloClientCast.variables};
  external castClient :
    generatedApolloClient =>
    {. "query": [@bs.meth] (queryObj => string), "mutate": [@bs.meth] (mutationObj => string)} =
    "%identity";
  [@bs.obj]
  external getJSQueryConfig :
    (~query: queryString, ~variables: ApolloClientCast.variables=?, unit) => queryObj =
    "";
  [@bs.obj]
  external getJSMutationConfig :
    (~mutation: queryString, ~variables: ApolloClientCast.variables=?, unit) => mutationObj =
    "";
};