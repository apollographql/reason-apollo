open ReasonApolloTypes;

module MutationFactory = (Config:Config) => {
    external cast : string => {. "data": Js.Json.t, "loading": bool} = "%identity";
    [@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";
    [@bs.module "react-apollo"] external mutationComponent: ReasonReact.reactClass = "Mutation";

    let graphqlMutationAST = [@bs] gql(Config.query);

    type response =
      | Loading
      | Called
      | Error(apolloError)
      | Data(Config.t)
      | NoData;

    type renderPropObj = {
      result: response,
      data: option(Config.t),
      loading: bool,
      error: option(apolloError),
      networkStatus: int,
    };

    type renderPropObjJS = {
      .
      "loading": Js.boolean,
      "called": Js.boolean,
      "data": Js.Nullable.t(Js.Json.t),
      "error": Js.Nullable.t(apolloError),
      "networkStatus": int,
      "variables": Js.Null_undefined.t(Js.Json.t),
    };

    type mutationOptions = {
      .
      "variables": Js.Nullable.t(Js.Json.t),
      "refetchQueries": array(string),
    };

    type apolloMutation =
      mutationOptions => Js.Promise.t(renderPropObjJS);

    let apolloDataToReason: renderPropObjJS => response = 
      apolloData =>
        switch (
          apolloData##called |> Js.to_bool,
          apolloData##loading |> Js.to_bool,
          apolloData##data |> Utils.getNonEmptyObj,
          apolloData##error |> Js.Nullable.to_opt
        ) {
        | (true, false, _, _) => Called
        | (_, true, _, _) => Loading
        | (false, false, Some(data), None) => Data(Config.parse(data))
        | (false, false, _, Some(error)) => Error(error)
        | (false, false, None, None) => NoData
        };

    let convertJsInputToReason = (apolloData: renderPropObjJS) => {
      result: apolloDataToReason(apolloData),
      data:
        switch (apolloData##data |> Utils.getNonEmptyObj) {
        | None => None
        | Some(data) =>
          switch (Config.parse(data)) {
          | parsedData => Some(parsedData)
          | exception _ => None
          }
        },
      error: apolloData##error |> Js.Nullable.to_opt,
      loading: apolloData##loading |> Js.to_bool,
      networkStatus: apolloData##networkStatus,
    };

    let make = 
    (
      ~variables: option(Js.Json.t)=?,
      ~onError: option(unit => unit)=?,
      ~onCompleted: option(unit => unit)=?,
      children: (apolloMutation, renderPropObj) => ReasonReact.reactElement
    )
     => ReasonReact.wrapJsForReason(
      ~reactClass=mutationComponent,
      ~props=Js.Nullable.(
        {
          "mutation": graphqlMutationAST,
          "variables": variables |> from_opt,
          "onError": onError |> from_opt,
          "onCompleted": onCompleted |> from_opt
        }
      ),
      (mutation, apolloData) =>
      children(mutation, convertJsInputToReason(apolloData))
    );
};
