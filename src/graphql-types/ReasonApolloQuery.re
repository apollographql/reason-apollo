open! ReasonApolloTypes;

type updateQueryOptions = {
  fetchMoreResult: option(Js.Json.t),
  variables: option(Js.Json.t),
};

type onErrorT;
type updateQueryT = (Js.Json.t, updateQueryOptions) => Js.Json.t;
type updateSubscriptionOptions = {
  subscriptionData: option(Js.Json.t),
  variables: option(Js.Json.t),
};
type updateQuerySubscriptionT =
  (Js.Json.t, updateSubscriptionOptions) => Js.Json.t;

type subscribeToMoreOptions = {
  document: queryString,
  variables: option(Js.Json.t),
  updateQuery: option(updateQuerySubscriptionT),
  onError: option(onErrorT),
};

type fetchMoreOptions = {
  variables: option(Js.Json.t),
  updateQuery: updateQueryT,
};

type renderPropObjJS = {
  loading: bool,
  data: Js.Nullable.t(Js.Json.t),
  error: Js.Nullable.t(apolloError),
  refetch: Js.Nullable.t(Js.Json.t) => Js.Promise.t(renderPropObjJS),
  networkStatus: Js.Nullable.t(int),
  variables: Js.Nullable.t(Js.Json.t),
  fetchMore: fetchMoreOptions => Js.Promise.t(unit),
  subscribeToMore: subscribeToMoreOptions => unit,
};

module Make = (Config: Config) => {
  [@bs.module "graphql-tag"] external gql: gql = "default";

  type response = queryResponse(Config.t);

  type renderPropObj = {
    result: response,
    data: option(Config.t),
    error: option(apolloError),
    loading: bool,
    refetch: option(Js.Json.t) => Js.Promise.t(response),
    fetchMore:
      (~variables: option(Js.Json.t), ~updateQuery: updateQueryT, unit) =>
      Js.Promise.t(unit),
    networkStatus: option(int),
    subscribeToMore:
      (
        ~document: queryString,
        ~variables: Js.Json.t=?,
        ~updateQuery: updateQuerySubscriptionT=?,
        ~onError: onErrorT=?,
        unit
      ) =>
      unit,
  };

  let graphqlQueryAST = gql(. Config.query);
  let apolloDataToVariant = (apolloData: renderPropObjJS): response =>
    switch (
      apolloData.loading,
      apolloData.data |> ReasonApolloUtils.getNonEmptyObj,
      apolloData.error |> Js.Nullable.toOption,
    ) {
    | (true, _, _) => Loading
    | (false, Some(response), _) => Data(Config.parse(response))
    | (false, _, Some(error)) => Error(error)
    | (false, None, None) =>
      Error({
        message: "No data",
        graphQLErrors: Js.Nullable.null,
        networkError: Js.Nullable.null,
      })
    };

  let convertJsInputToReason = (apolloData: renderPropObjJS) => {
    result: apolloData |> apolloDataToVariant,
    data:
      switch (apolloData.data |> ReasonApolloUtils.getNonEmptyObj) {
      | None => None
      | Some(data) =>
        switch (Config.parse(data)) {
        | parsedData => Some(parsedData)
        | exception _ => None
        }
      },
    error:
      switch (apolloData.error |> Js.Nullable.toOption) {
      | Some(error) => Some(error)
      | None => None
      },
    loading: apolloData.loading,
    refetch: variables =>
      apolloData.refetch(variables |> Js.Nullable.fromOption)
      |> Js.Promise.then_(data =>
           data |> apolloDataToVariant |> Js.Promise.resolve
         ),
    fetchMore: (~variables, ~updateQuery, ()) =>
      apolloData.fetchMore({variables, updateQuery}),
    networkStatus: apolloData.networkStatus->Js.Nullable.toOption,
    subscribeToMore:
      (~document, ~variables=?, ~updateQuery=?, ~onError=?, ()) => {
      apolloData.subscribeToMore({document, variables, updateQuery, onError});
    },
  };

  module JsQuery = {
    [@bs.module "react-apollo"] [@react.component]
    external make:
      (
        ~query: queryString,
        ~variables: Js.Json.t=?,
        ~pollInterval: int=?,
        ~notifyOnNetworkStatusChange: bool=?,
        ~fetchPolicy: string=?,
        ~errorPolicy: string=?,
        ~ssr: bool=?,
        ~displayName: string=?,
        ~skip: bool=?,
        ~onCompleted: Js.Nullable.t(Js.Json.t) => unit=?,
        ~onError: apolloError => unit=?,
        ~partialRefetch: bool=?,
        ~delay: bool=?,
        ~context: Js.Json.t=?,
        ~children: renderPropObjJS => React.element
      ) =>
      React.element =
      "Query";
  };

  [@react.component]
  let make =
      (
        ~variables=?,
        ~pollInterval=?,
        ~notifyOnNetworkStatusChange=?,
        ~fetchPolicy=?,
        ~errorPolicy=?,
        ~ssr=?,
        ~displayName=?,
        ~skip=?,
        ~onCompleted=?,
        ~onError=?,
        ~partialRefetch=?,
        ~delay=?,
        ~context=?,
        ~children,
      ) =>
    <JsQuery
      query=graphqlQueryAST
      ?variables
      ?pollInterval
      ?notifyOnNetworkStatusChange
      ?fetchPolicy
      ?errorPolicy
      ?ssr
      ?displayName
      ?skip
      ?onCompleted
      ?onError
      ?partialRefetch
      ?delay
      ?context>
      {apolloData => apolloData |> convertJsInputToReason |> children}
    </JsQuery>;
};
