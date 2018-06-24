open ReasonApolloTypes;

module Get = (Config: ReasonApolloTypes.Config) => {
  [@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";
  [@bs.module "react-apollo"] external subscriptionComponent: ReasonReact.reactClass = "Subscription";

  let graphQLSubscriptionAST = gql(. Config.query);
  
  type response = 
    | Loading
    | Error(apolloError)
    | Data(Config.t);

  type renderPropObj = {
    result: response,
    data: option(Config.t),
    error: option(apolloError),
    loading: bool
  };

  type renderPropObjJS = {
    .
    "loading": bool,
    "data": Js.Nullable.t(Js.Json.t),
    "error": Js.Nullable.t(apolloError)
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

  let convertJsInputToReason: renderPropObjJS => renderPropObj 
    = (apolloData) => {
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

  let make = (
    ~variables: option(Js.Json.t)=?,
    ~children: renderPropObj => ReasonReact.reactElement,
    ) => 
    ReasonReact.wrapJsForReason(
      ~reactClass=subscriptionComponent,
      ~props=
        Js.Nullable.(
          {
            "subscription": graphQLSubscriptionAST,
            "variables": variables |> fromOption
          }  
        ),
      apolloData => apolloData |> convertJsInputToReason |> children
    );
};
