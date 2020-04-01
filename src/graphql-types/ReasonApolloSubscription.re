open! ReasonApolloTypes;

module Make = (Config: ReasonApolloTypes.Config) => {
  [@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

  let graphQLSubscriptionAST = gql(. Config.query);

  type response = subscriptionResponse(Config.t);

  type renderPropObj = {
    result: response,
    data: option(Config.t),
    error: option(apolloError),
    loading: bool,
  };

  type renderPropObjJS = {
    loading: bool,
    data: Js.Nullable.t(Js.Json.t),
    error: Js.Nullable.t(apolloError),
  };

  let apolloDataToVariant = apolloData =>
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

  let convertJsInputToReason = apolloData => {
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
  };

  module JsSubscription = {
    [@bs.module "react-apollo"] [@react.component]
    external make:
      (
        ~subscription: ReasonApolloTypes.queryString,
        ~variables: option(Js.Json.t),
        ~children: renderPropObjJS => ReasonReact.reactElement
      ) =>
      ReasonReact.reactElement =
      "Subscription";
  };

  [@react.component]
  let make = (~variables=?, ~children) =>
    <JsSubscription subscription=graphQLSubscriptionAST variables>
      {apolloData => apolloData |> convertJsInputToReason |> children}
    </JsSubscription>;
};
