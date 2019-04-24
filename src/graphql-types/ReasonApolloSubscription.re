open ReasonApolloTypes;

module Make = (Config: ReasonApolloTypes.Config) => {
  [@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
  [@bs.module "react-apollo"]
  external subscriptionComponent: ReasonReact.reactClass = "Subscription";

  let graphQLSubscriptionAST = gql(. Config.query);

  type response = subscriptionResponse(Config.t);

  type renderPropObj = {
    result: response,
    data: option(Config.t),
    error: option(apolloError),
    loading: bool,
  };

  type renderPropObjJS = {
    .
    "loading": bool,
    "data": Js.Nullable.t(Js.Json.t),
    "error": Js.Nullable.t(apolloError),
  };

  let apolloDataToVariant: renderPropObjJS => response =
    apolloData =>
      switch (
        apolloData##loading,
        apolloData##data |> Js.Nullable.toOption,
        apolloData##error |> Js.Nullable.toOption,
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

  let convertJsInputToReason: renderPropObjJS => renderPropObj =
    apolloData => {
      result: apolloData |> apolloDataToVariant,
      data:
        switch (apolloData##data |> ReasonApolloUtils.getNonEmptyObj) {
        | None => None
        | Some(data) =>
          switch (Config.parse(data)) {
          | parsedData => Some(parsedData)
          | exception _ => None
          }
        },
      error:
        switch (apolloData##error |> Js.Nullable.toOption) {
        | Some(error) => Some(error)
        | None => None
        },
      loading: apolloData##loading,
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

  module Subscription = {
    [@bs.module "react-apollo"] [@react.component]
    external make:
      (
        ~subscription: queryString,
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
      "Subscription";
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
    <Subscription
      subscription=graphQLSubscriptionAST
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
    </Subscription>;
  };
};
