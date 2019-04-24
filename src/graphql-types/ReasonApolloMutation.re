open ReasonApolloTypes;

type renderPropObjJS = {
  .
  "loading": bool,
  "called": bool,
  "data": Js.Nullable.t(Js.Json.t),
  "error": Js.Nullable.t(apolloError),
  "networkStatus": Js.Nullable.t(int),
  "variables": Js.Null_undefined.t(Js.Json.t),
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
  [@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
  [@bs.module "react-apollo"]
  external mutationComponent: ReasonReact.reactClass = "Mutation";
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

  [@bs.obj]
  external makeMutateParams:
    (
      ~variables: Js.Json.t=?,
      ~refetchQueries: array(string)=?,
      ~optimisticResponse: Config.t=?
    ) =>
    _ =
    "";

  let convertExecutionResultToReason = (executionResult: executionResult) =>
    switch (
      executionResult##data |> ReasonApolloUtils.getNonEmptyObj,
      executionResult##errors |> Js.Nullable.toOption,
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
    jsMutation(
      makeMutateParams(~variables?, ~refetchQueries?, ~optimisticResponse?),
    )
    |> Js.Promise.(
         then_(response => resolve(convertExecutionResultToReason(response)))
       );
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
    networkStatus: apolloData##networkStatus->Js.Nullable.toOption,
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
  type children = (apolloMutation, renderPropObj) => React.element;

  module Mutation = {
    [@bs.module "react-apollo"] [@react.component]
    external make:
      (
        ~mutation: queryString,
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
        ~children: ('a, 'b) => React.element
      ) =>
      React.element =
      "Mutation";
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
        ~children: (apolloMutation, renderPropObj) => React.element,
      ) => {
    <Mutation
      mutation=graphqlMutationAST
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
      {(mutation, apolloData) =>
         children(
           apolloMutationFactory(~jsMutation=mutation),
           apolloData |> convertJsInputToReason,
         )}
    </Mutation>;
  };
};
