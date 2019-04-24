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
  subscribeToMore: (subscribeToMoreOptions, unit) => unit,
};

module Make = (Config: ReasonApolloTypes.Config) => {
  open Js.Nullable;

  [@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";

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
        unit,
        unit
      ) =>
      unit,
  };

  let graphqlQueryAST = gql(. Config.query);

  let apolloDataToVariant: renderPropObjJS => response =
    apolloData =>
      switch (
        apolloData->loadingGet,
        apolloData->dataGet |> Js.Nullable.toOption,
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

  type variables = Js.Json.t;
  type pollInterval = int;
  type notifyOnNetworkStatusChange = bool;
  type fetchPolicy = string;
  type errorPolicy = string;
  type ssr = bool;
  type displayName = string;
  type skip = bool;
  type onCompleted = Js.Nullable.t(Js.Json.t) => unit;
  type onError = apolloError => unit;
  type partialRefetch = bool;
  type delay = bool;
  type context = Js.Json.t;
  type children = renderPropObj => React.element;

  module Query = {
    [@bs.module "react-apollo"] [@react.component]
    external make:
      (
        ~query: queryString,
        ~variables: option(Js.Json.t)=?,
        ~pollInterval: option(pollInterval)=?,
        ~notifyOnNetworkStatusChange: option(notifyOnNetworkStatusChange)=?,
        ~fetchPolicy: option(fetchPolicy)=?,
        ~errorPolicy: option(errorPolicy)=?,
        ~ssr: option(ssr)=?,
        ~displayName: option(displayName)=?,
        ~skip: option(skip)=?,
        ~onCompleted: option(onCompleted)=?,
        ~onError: option(onError)=?,
        ~partialRefetch: option(partialRefetch)=?,
        ~delay: option(delay)=?,
        ~context: option(context)=?,
        ~children: 'a => React.element
      ) =>
      React.element =
      "Query";
  };

  [@react.component]
  let make =
      (
        ~variables: option(Js.Json.t)=?,
        ~pollInterval: option(pollInterval)=?,
        ~notifyOnNetworkStatusChange: option(notifyOnNetworkStatusChange)=?,
        ~fetchPolicy: option(fetchPolicy)=?,
        ~errorPolicy: option(errorPolicy)=?,
        ~ssr: option(ssr)=?,
        ~displayName: option(displayName)=?,
        ~skip: option(skip)=?,
        ~onCompleted: option(onCompleted)=?,
        ~onError: option(onError)=?,
        ~partialRefetch: option(partialRefetch)=?,
        ~delay: option(delay)=?,
        ~context: option(context)=?,
        ~children: renderPropObj => React.element,
      ) => {
    <Query
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
    </Query>;
  };
};
