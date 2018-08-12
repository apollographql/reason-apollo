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

module type ReadFragment = (Config : ReasonApolloTypes.Config) => {
  let read : (generatedApolloClient, string) => option(Config.t);
};

type readFragmentObj = {
  .
  "id": string,
  "fragment": ReasonApolloTypes.queryString,
};

[@bs.send] external readFragment : (generatedApolloClient, readFragmentObj) => Js.Nullable.t(Js.Json.t) = "";

module ReadFragment(Config : ReasonApolloTypes.Config) = {
  [@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

  let read = (client : generatedApolloClient, id : string) : option(Config.t) =>
    client
    |> readFragment(_, {"id": id, "fragment": gql(. Config.query)})
    |> Js.Nullable.toOption
    |> (fragmentDataOption) =>
         switch(fragmentDataOption) {
         | None => None
         | Some(fragmentData) => Some(fragmentData |> Config.parse)
         };
};