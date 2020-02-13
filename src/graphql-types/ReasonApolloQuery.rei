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

/* We don't accept a new query for now */

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

module Make:
  (Config: Config) =>
   {
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

    let graphqlQueryAST: queryString;
    let apolloDataToVariant: renderPropObjJS => response;

    let convertJsInputToReason: renderPropObjJS => renderPropObj;

    module JsQuery: {
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
    let make:
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
        ~children: renderPropObj => React.element
      ) =>
      React.element;
  };
