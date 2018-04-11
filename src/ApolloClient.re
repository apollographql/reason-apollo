open ReasonApolloTypes;

type queryObj = {. "query": ReasonApolloTypes.queryString, "variables": Js.Json.t };
type mutationObj = {. "mutation": ReasonApolloTypes.queryString, "variables": Js.Json.t};

type generatedApolloClient = {. 
  "query": [@bs.meth] (queryObj => string),
  "mutate": [@bs.meth] (mutationObj => string)
};

type clientOptions = {
  .
  "link": apolloLink,
  "cache": apolloCache,
  "ssrMode": Js.Nullable.t(Js.boolean),
  "ssrForceFetchDelay": Js.Nullable.t(int),
  "connectToDevTools": Js.Nullable.t(Js.boolean),
  "queryDeduplication": Js.Nullable.t(Js.boolean)
};

type fetch;

type linkOptions = {
  .
  "uri": string,
  "includeExtensions": Js.Nullable.t(Js.boolean),
  "fetch": Js.Nullable.t(fetch),
  "headers": Js.Nullable.t(Js.Json.t),
  "credentials": Js.Nullable.t(string),
  "fetchOptions": Js.Nullable.t(Js.Json.t)
};