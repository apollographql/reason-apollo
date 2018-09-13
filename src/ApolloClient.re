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

module ReadQuery = (Config: ReasonApolloTypes.Config) => {
  [@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
  type readQueryOptions = {
    .
    "query": queryString,
    "variables": Js.Nullable.t(Js.Json.t),
  };
  [@bs.send]
  external readQuery: (generatedApolloClient, readQueryOptions) => Config.t =
    "";
  type response = option(Config.t);

  let graphqlQueryAST = gql(. Config.query);
  let apolloDataToRecord: Js.Nullable.t(Js.Json.t) => response =
    apolloData =>
      Js.Nullable.toOption(apolloData)->(Belt.Option.map(Config.parse));

  let make = (~client, ~variables: option(Js.Json.t)=?, ()) =>
    readQuery(
      client,
      {
        "query": graphqlQueryAST,
        "variables": Js.Nullable.fromOption(variables),
      },
    );
};

module WriteQuery = (Config: ReasonApolloTypes.Config) => {
  [@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
  type writeQueryOptions = {
    .
    "query": ReasonApolloTypes.queryString,
    "variables": Js.Nullable.t(Js.Json.t),
    "data": Config.t,
  };
  [@bs.send]
  external writeQuery: (generatedApolloClient, writeQueryOptions) => unit = "";

  let graphqlQueryAST = gql(. Config.query);

  let make = (~client, ~variables: option(Js.Json.t)=?, ~data: Config.t, ()) =>
    writeQuery(
      client,
      {
        "query": graphqlQueryAST,
        "variables": Js.Nullable.fromOption(variables),
        "data": data,
      },
    );
};
