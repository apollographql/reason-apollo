open ReasonApolloTypes;

type queryObj = {
  .
  "query": ReasonApolloTypes.queryString,
  "variables": Js.Json.t,
};

type mutationObj = {
  .
  "mutation": ReasonApolloTypes.queryString,
  "variables": Js.Json.t,
};

type generatedApolloClient = {
  .
  "query": [@bs.meth] (queryObj => Js.Promise.t(ReasonApolloQuery.renderPropObjJS)),
  "mutate": [@bs.meth] (mutationObj => Js.Promise.t(ReasonApolloMutation.renderPropObjJS)),
  "resetStore": [@bs.meth] (unit => unit),
};

type fetch;

type linkOptions = {
  .
  "uri": string,
  "includeExtensions": Js.Nullable.t(bool),
  "fetch": Js.Nullable.t(fetch),
  "headers": Js.Nullable.t(Js.Json.t),
  "credentials": Js.Nullable.t(string),
  "fetchOptions": Js.Nullable.t(Js.Json.t),
};

type uploadLinkOptions = {
  .
  "uri": Js.Nullable.t(string),
  "fetch": Js.Nullable.t(fetch),
  "fetchOptions": Js.Nullable.t(Js.t({.})),
  "credentials": Js.Nullable.t(string),
  "headers": Js.Nullable.t(Js.Json.t),
  "includeExtensions": Js.Nullable.t(bool),
};

[@bs.module "apollo-client"] [@bs.new]
external createApolloClientJS : 'a => generatedApolloClient = "ApolloClient";

[@bs.obj]
external apolloClientObjectParam :
  (
    ~link: apolloLink,
    ~cache: apolloCache,
    ~ssrMode: bool=?,
    ~ssrForceFetchDelay: int=?,
    ~connectToDevTools: bool=?,
    ~queryDeduplication: bool=?
  ) =>
  _ =
  "";