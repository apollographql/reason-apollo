open ReasonApolloTypes;

module Get = (Config: ReasonApolloTypes.Config) => {
  [@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";
  [@bs.module "react-apollo"]
  external queryComponent : ReasonReact.reactClass = "Query";
  type response =
    | Loading
    | Error(apolloError)
    | Data(Config.t);

  [@bs.deriving abstract]
  type updateQueryOptions = {
    [@bs.optional]
    fetchMoreResult: Js.Json.t,
    [@bs.optional]
    variables: Js.Json.t,
  };

  type updateQueryT = (Js.Json.t, updateQueryOptions) => Js.Json.t;

  /* We dont accept a new query for now */
  [@bs.deriving abstract]
  type fetchMoreOptions = {
    [@bs.optional]
    variables: Js.Json.t,
    updateQuery: updateQueryT,
  };
  
  type updateSubscriptionOptions = {
    .
    "subscriptionData": Js.Nullable.t(Js.Json.t),
    "variables": Js.Nullable.t(Js.Json.t),
  };

  type updateQuerySubscriptionT = (Js.Json.t, updateSubscriptionOptions) => Js.Json.t;
  type onErrorT;

  [@bs.deriving abstract] 
  type subscribeToMoreOptions = {
    document: queryString,
    [@bs.optional] variables: Js.Json.t,
    [@bs.optional] updateQuery: updateQuerySubscriptionT,
    [@bs.optional] onError: onErrorT 
  };

  type renderPropObj = {
    result: response,
    data: option(Config.t),
    error: option(apolloError),
    loading: bool,
    refetch: option(Js.Json.t) => Js.Promise.t(response),
    fetchMore:
      (~variables: Js.Json.t=?, ~updateQuery: updateQueryT, unit) =>
      Js.Promise.t(unit),
    networkStatus: int,
    subscribeToMore: (~document: queryString, ~variables: Js.Json.t=?, ~updateQuery: updateQuerySubscriptionT=?, ~onError: onErrorT=?, unit) => unit => unit
  };

  [@bs.deriving abstract]
  type renderPropObjJS = {
    loading: bool,
    data: Js.Nullable.t(Js.Json.t),
    error: Js.Nullable.t(apolloError),
    refetch: Js.Null_undefined.t(Js.Json.t) => Js.Promise.t(renderPropObjJS),
    networkStatus: int,
    variables: Js.Null_undefined.t(Js.Json.t),
    fetchMore: fetchMoreOptions => Js.Promise.t(unit),
    subscribeToMore: subscribeToMoreOptions => unit => unit,
  };

  let graphqlQueryAST = gql(. Config.query);
  let apolloDataToVariant: renderPropObjJS => response =
    apolloData =>
      switch (
        apolloData |. loading,
        apolloData |. data |> Js.Nullable.toOption,
        apolloData |. error |> Js.Nullable.toOption,
      ) {
      | (true, _, _) => Loading
      | (false, Some(response), _) => Data(Config.parse(response))
      | (false, _, Some(error)) => Error(error)
      | (false, None, None) =>
        Error({
          "message": "No data",
          "graphQLErrors": Js.Nullable.null,
          "networkError": Js.Nullable.null,
        })
      };

  let convertJsInputToReason = (apolloData: renderPropObjJS) => {
    result: apolloData |> apolloDataToVariant,
    data:
      switch (apolloData |. data |> ReasonApolloUtils.getNonEmptyObj) {
      | None => None
      | Some(data) =>
        switch (Config.parse(data)) {
        | parsedData => Some(parsedData)
        | exception _ => None
        }
      },
    error:
      switch (apolloData |. error |> Js.Nullable.toOption) {
      | Some(error) => Some(error)
      | None => None
      },
    loading: apolloData |. loading,
    refetch: variables =>
      apolloData |. refetch(variables |> Js.Nullable.fromOption)
      |> Js.Promise.then_(data =>
           data |> apolloDataToVariant |> Js.Promise.resolve
         ),
    fetchMore: (~variables=?, ~updateQuery, ()) =>
      apolloData |. fetchMore(fetchMoreOptions(~variables?, ~updateQuery, ())),
    networkStatus: apolloData |.networkStatus,
    subscribeToMore: (~document, ~variables=?, ~updateQuery=?, ~onError=?, ()) => 
    apolloData |. subscribeToMore(subscribeToMoreOptions(
      ~document,
      ~variables?,
      ~updateQuery?,
      ~onError?,
      ()
    )),
  };

  let make =
      (
        ~variables: option(Js.Json.t)=?,
        ~pollInterval: option(int)=?,
        ~notifyOnNetworkStatusChange: option(bool)=?,
        ~fetchPolicy: option(string)=?,
        ~errorPolicy: option(string)=?,
        ~ssr: option(bool)=?,
        ~displayName: option(string)=?,
        ~delay: option(bool)=?,
        ~context: option(Js.Json.t)=?,
        children: renderPropObj => ReasonReact.reactElement,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=queryComponent,
      ~props=
        Js.Nullable.(
          {
            "query": graphqlQueryAST,
            "variables": variables |> fromOption,
            "pollInterval": pollInterval |> fromOption,
            "notifyOnNetworkStatusChange":
              notifyOnNetworkStatusChange |> fromOption,
            "fetchPolicy": fetchPolicy |> fromOption,
            "errorPolicy": errorPolicy |> fromOption,
            "ssr": ssr |> fromOption,
            "displayName": displayName |> fromOption,
            "delay": delay |> fromOption,
            "context": context |> fromOption,
          }
        ),
      apolloData =>
      apolloData |> convertJsInputToReason |> children
    );
};
