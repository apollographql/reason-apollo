open! ReasonApolloTypes;

type renderPropObjJS = {
  loading: bool,
  called: bool,
  data: Js.Nullable.t(Js.Json.t),
  error: Js.Nullable.t(apolloError),
  networkStatus: Js.Nullable.t(int),
  variables: Js.Null_undefined.t(Js.Json.t),
};

module Make:
  (Config: Config) =>
   {
    external cast:
      string =>
      {
        .
        "data": Js.Json.t,
        "loading": bool,
      } =
      "%identity";

    [@bs.module "graphql-tag"] external gql: gql = "default";

    let graphqlMutationAST: queryString;
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

    let convertExecutionResultToReason:
      executionResult => executionResponse(Config.t);

    let apolloMutationFactory:
      (
        ~jsMutation: jsMutationParams => Js.Promise.t(executionResult),
        ~variables: Js.Json.t=?,
        ~refetchQueries: array(string)=?,
        ~optimisticResponse: Config.t=?,
        unit
      ) =>
      Js.Promise.t(executionResponse(Config.t));

    let apolloDataToReason: renderPropObjJS => response;

    let convertJsInputToReason: renderPropObjJS => renderPropObj;

    module JsMutation: {
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
    let make:
      (
        ~variables: Js.Json.t=?,
        ~onError: apolloError => unit=?,
        ~onCompleted: unit => unit=?,
        ~children: (apolloMutation, renderPropObj) => React.element
      ) =>
      React.element;
  };
