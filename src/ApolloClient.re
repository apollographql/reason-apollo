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
  "query": [@bs.meth] (queryObj => string),
  "mutate": [@bs.meth] (mutationObj => string),
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

module ReadFragment(Config : ReasonApolloTypes.Config) = {
  [@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

  type readFragmentObj = {
    .
    "id": string,
    "fragment": ReasonApolloTypes.queryString,
  };

  [@bs.send] external readFragment : (ApolloClient.generatedApolloClient, readFragmentObj) => Js.Json.t = "";

  let readFragment = (client, id : string) =>
    readFragment(client, {"id": id, "fragment": gql(. Config.query)});
};