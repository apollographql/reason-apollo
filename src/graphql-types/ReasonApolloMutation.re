open! ReasonApolloTypes;

type renderPropObjJS = {
  loading: bool,
  called: bool,
  data: Js.Nullable.t(Js.Json.t),
  error: Js.Nullable.t(apolloError),
  networkStatus: Js.Nullable.t(int),
  variables: Js.Null_undefined.t(Js.Json.t),
};

module Make = (Config: Config) => {
  external cast:
    string =>
    {
      .
      "data": Js.Json.t,
      "loading": bool,
    } =
    "%identity";

  [@bs.module "graphql-tag"] external gql: gql = "default";

  let graphqlMutationAST = gql(. Config.query);
  type response = mutationResponse(Config.t);
  type renderPropObj = {
    result: response,
    data: option(Config.t),
    loading: bool,
    error: option(apolloError),
    networkStatus: option(int),
  };
  type apolloMutation =
    (
      ~variables: Js.Json.t=?,
      ~refetchQueries: array(string)=?,
      ~optimisticResponse: Config.t=?,
      unit
    ) =>
    Js.Promise.t(executionResponse(Config.t));

  type jsMutationParams = {
    variables: option(Js.Json.t),
    refetchQueries: option(array(string)),
    optimisticResponse: option(Config.t),
  };

  let convertExecutionResultToReason = (executionResult: executionResult) =>
    switch (
      executionResult.data |> ReasonApolloUtils.getNonEmptyObj,
      executionResult.errors |> Js.Nullable.toOption,
    ) {
    | (Some(data), _) => Data(Config.parse(data))
    | (_, Some(errors)) => Errors(errors)
    | (None, None) => EmptyResponse
    };

  let apolloMutationFactory =
      (
        ~jsMutation,
        ~variables=?,
        ~refetchQueries=?,
        ~optimisticResponse=?,
        (),
      ) =>
    jsMutation({variables, refetchQueries, optimisticResponse})
    // jsMutation(
    //   jsMutationParams(
    //     ~variables?,
    //     ~refetchQueries?,
    //     ~optimisticResponse?,
    //     (),
    //   ),
    // )
    |> Js.Promise.(
         then_(response => resolve(convertExecutionResultToReason(response)))
       );

  let apolloDataToReason: renderPropObjJS => response =
    apolloData =>
      switch (
        apolloData.loading,
        apolloData.data |> ReasonApolloUtils.getNonEmptyObj,
        apolloData.error |> Js.Nullable.toOption,
      ) {
      | (true, _, _) => Loading
      | (false, Some(data), _) => Data(Config.parse(data))
      | (false, _, Some(error)) => Error(error)
      | (false, None, None) => NotCalled
      };

  let convertJsInputToReason = (apolloData: renderPropObjJS) => {
    result: apolloDataToReason(apolloData),
    data:
      switch (apolloData.data |> ReasonApolloUtils.getNonEmptyObj) {
      | None => None
      | Some(data) =>
        switch (Config.parse(data)) {
        | parsedData => Some(parsedData)
        | exception _ => None
        }
      },
    error: apolloData.error |> Js.Nullable.toOption,
    loading: apolloData.loading,
    networkStatus: apolloData.networkStatus->Js.Nullable.toOption,
  };

  module JsMutation = {
    [@bs.module "react-apollo"] [@react.component]
    external make:
      (
        ~mutation: queryString,
        ~variables: Js.Json.t=?,
        ~onCompleted: unit => unit=?,
        ~onError: apolloError => unit=?,
        ~children: (
                     jsMutationParams => Js.Promise.t(executionResult),
                     renderPropObjJS
                   ) =>
                   React.element
      ) =>
      React.element =
      "Mutation";
  };

  [@react.component]
  let make = (~variables=?, ~onError=?, ~onCompleted=?, ~children) =>
    <JsMutation mutation=graphqlMutationAST ?variables ?onError ?onCompleted>
      {(mutation, apolloData) =>
         children(
           apolloMutationFactory(~jsMutation=mutation),
           apolloData |> convertJsInputToReason,
         )}
    </JsMutation>;
};
