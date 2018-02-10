open ReasonApolloTypes;

type queryObj = {
  .
  "query": ReasonApolloTypes.queryString,
  "variables": Js.Json.t
};

type queryObservableObj = {
  .
  "query": ReasonApolloTypes.queryString,
  "variables": Js.Json.t
};

type result = {
  .
  "data": Js.nullable(Js.Json.t),
  "error": Js.nullable(string),
  "loading": Js.boolean
};

type subscription = {. [@bs.meth] "unsubscribe": unit => unit};

type watchQueryObservable = {
  .
  [@bs.meth] "currentResult":  (unit => result),
  [@bs.meth] "result": unit => Js.Promise.t(result),
  [@bs.meth]
  "subscribe": {
    .
    "next": result => unit,
    "error": Js.Exn.t => unit
  } => subscription,
  [@bs.meth] "getLastError": [@bs.return nullable] (unit => option(Js.Exn.t))
};

type mutationObj = {
  .
  "mutation": ReasonApolloTypes.queryString,
  "variables": Js.Json.t
};

type generatedApolloClient = {
  .
  "query": [@bs.meth] (queryObj => Js.Promise.t(result)),
  "watchQuery": [@bs.meth] (queryObservableObj => watchQueryObservable),
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

[@bs.module "apollo-client"] [@bs.new]
external createApolloClient : clientOptions => generatedApolloClient =
  "ApolloClient";
