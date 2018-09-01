open ReasonApolloTypes;

type renderPropObjJS = {
  .
  "loading": bool,
  "called": bool,
  "data": Js.Nullable.t(Js.Json.t),
  "error": Js.Nullable.t(apolloError),
  "networkStatus": int,
  "variables": Js.Null_undefined.t(Js.Json.t),
};

module MutationFactory = (Config: Config) => {
  external cast :
    string =>
    {
      .
      "data": Js.Json.t,
      "loading": bool,
    } =
    "%identity";
  [@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";
  [@bs.module "react-apollo"]
  external mutationComponent : ReasonReact.reactClass = "Mutation";
  let graphqlMutationAST = gql(. Config.query);
  type response = mutationResponse(Config.t);
  type renderPropObj = {
    result: response,
    data: option(Config.t),
    loading: bool,
    error: option(apolloError),
    networkStatus: int,
  };
  type apolloMutation =
    (~variables: Js.Json.t=?, ~refetchQueries: array(string)=?, unit) =>
    Js.Promise.t(renderPropObjJS);
  [@bs.obj]
  external makeMutateParams :
    (~variables: Js.Json.t=?, ~refetchQueries: array(string)=?) => _ =
    "";
  let apolloMutationFactory =
      (~jsMutation, ~variables=?, ~refetchQueries=?, ()) =>
    jsMutation(makeMutateParams(~variables?, ~refetchQueries?));
  let apolloDataToReason: renderPropObjJS => response =
    apolloData =>
      switch (
        apolloData##loading,
        apolloData##data |> ReasonApolloUtils.getNonEmptyObj,
        apolloData##error |> Js.Nullable.toOption,
      ) {
      | (true, _, _) => Loading
      | (false, Some(data), _) => Data(Config.parse(data))
      | (false, _, Some(error)) => Error(error)
      | (false, None, None) => NotCalled
      };
  let convertJsInputToReason = (apolloData: renderPropObjJS) => {
    result: apolloDataToReason(apolloData),
    data:
      switch (apolloData##data |> ReasonApolloUtils.getNonEmptyObj) {
      | None => None
      | Some(data) =>
        switch (Config.parse(data)) {
        | parsedData => Some(parsedData)
        | exception _ => None
        }
      },
    error: apolloData##error |> Js.Nullable.toOption,
    loading: apolloData##loading,
    networkStatus: apolloData##networkStatus,
  };
  let make =
      (
        ~variables: option(Js.Json.t)=?,
        ~onError: option(unit => unit)=?,
        ~onCompleted: option(unit => unit)=?,
        children: (apolloMutation, renderPropObj) => ReasonReact.reactElement,
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=mutationComponent,
      ~props=
        Js.Nullable.(
          {
            "mutation": graphqlMutationAST,
            "variables": variables |> fromOption,
            "onError": onError |> fromOption,
            "onCompleted": onCompleted |> fromOption,
          }
        ),
      (mutation, apolloData) =>
      children(
        apolloMutationFactory(~jsMutation=mutation),
        convertJsInputToReason(apolloData),
      )
    );
};
