open ReasonApolloTypes;

type variableTypeToBeDefined;

type queryObj = {. "query": queryString, "variables": variableTypeToBeDefined};

type generatedApolloClient = {. "query": [@bs.meth] (queryObj => string)};

type linkOptions = {. "uri": string};

[@bs.obj]
external clientOptions :
  (
    ~ssrMode: Js.boolean=?,
    ~ssrForceFetchDelay: int=?,
    ~link: apolloLink,
    ~cache: apolloCache,
    unit
  ) =>
  'a =
  "";

[@bs.module "apollo-client"] [@bs.new]
external apolloClient : clientOptions => generatedApolloClient =
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