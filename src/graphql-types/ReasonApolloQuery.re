open ReasonApolloTypes;

[@bs.deriving abstract]
type updateQueryOptions = {
  [@bs.optional]
  fetchMoreResult: Js.Json.t,
  [@bs.optional]
  variables: Js.Json.t,
};

type onErrorT;
type updateQueryT = (Js.Json.t, updateQueryOptions) => Js.Json.t;
type updateSubscriptionOptions = {
  .
  "subscriptionData": Js.Nullable.t(Js.Json.t),
  "variables": Js.Nullable.t(Js.Json.t),
};
type updateQuerySubscriptionT =
  (Js.Json.t, updateSubscriptionOptions) => Js.Json.t;

[@bs.deriving abstract]
type subscribeToMoreOptions = {
  document: queryString,
  [@bs.optional]
  variables: Js.Json.t,
  [@bs.optional]
  updateQuery: updateQuerySubscriptionT,
  [@bs.optional]
  onError: onErrorT,
};

/* We dont accept a new query for now */
[@bs.deriving abstract]
type fetchMoreOptions = {
  [@bs.optional]
  variables: Js.Json.t,
  updateQuery: updateQueryT,
};

[@bs.deriving abstract]
type renderPropObjJS = {
  loading: bool,
  data: Js.Nullable.t(Js.Json.t),
  error: Js.Nullable.t(apolloError),
  refetch: Js.Null_undefined.t(Js.Json.t) => Js.Promise.t(renderPropObjJS),
  networkStatus: Js.Nullable.t(int),
  variables: Js.Null_undefined.t(Js.Json.t),
  fetchMore: fetchMoreOptions => Js.Promise.t(unit),
  subscribeToMore: subscribeToMoreOptions => unit,
};

module Make = (Config: ReasonApolloTypes.Config) => {
  [@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

  type response = queryResponse(Config.t);

  type renderPropObj = {
    result: response,
    data: option(Config.t),
    error: option(apolloError),
    loading: bool,
    refetch: option(Js.Json.t) => Js.Promise.t(response),
    fetchMore:
      (~variables: Js.Json.t=?, ~updateQuery: updateQueryT, unit) =>
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
  let apolloDataToVariant: renderPropObjJS => response =
    apolloData =>
      switch (
        apolloData->loadingGet,
        apolloData->dataGet |> ReasonApolloUtils.getNonEmptyObj,
        apolloData->errorGet |> Js.Nullable.toOption,
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
      switch (apolloData->dataGet |> ReasonApolloUtils.getNonEmptyObj) {
      | None => None
      | Some(data) =>
        switch (Config.parse(data)) {
        | parsedData => Some(parsedData)
        | exception _ => None
        }
      },
    error:
      switch (apolloData->errorGet |> Js.Nullable.toOption) {
      | Some(error) => Some(error)
      | None => None
      },
    loading: apolloData->loadingGet,
    refetch: variables =>
      apolloData->(refetchGet(variables |> Js.Nullable.fromOption))
      |> Js.Promise.then_(data =>
           data |> apolloDataToVariant |> Js.Promise.resolve
         ),
    fetchMore: (~variables=?, ~updateQuery, ()) =>
      apolloData->(
                    fetchMoreGet(
                      fetchMoreOptions(~variables?, ~updateQuery, ()),
                    )
                  ),
    networkStatus: apolloData->networkStatusGet->Js.Nullable.toOption,
    subscribeToMore:
      (~document, ~variables=?, ~updateQuery=?, ~onError=?, ()) =>
      apolloData->(
                    subscribeToMoreGet(
                      subscribeToMoreOptions(
                        ~document,
                        ~variables?,
                        ~updateQuery?,
                        ~onError?,
                        (),
                      ),
                    )
                  ),
  };

  module JsQuery = {
    [@bs.module "react-apollo"] [@react.component]
    external make:
      (
        ~query: ReasonApolloTypes.queryString,
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
        ~children: renderPropObj => React.element,
      ) =>
    <JsQuery
      query=graphqlQueryAST
      variables
      pollInterval
      notifyOnNetworkStatusChange
      fetchPolicy
      errorPolicy
      ssr
      displayName
      skip
      onCompleted
      onError
      partialRefetch
      delay
      context>
      {apolloData => apolloData |> convertJsInputToReason |> children}
    </JsQuery>;
};
