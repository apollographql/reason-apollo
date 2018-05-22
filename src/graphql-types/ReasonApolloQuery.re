open ReasonApolloTypes;

module Get = (Config:ReasonApolloTypes.Config) => {    
    [@bs.module "graphql-tag"] external gql : ReasonApolloTypes.gql = "default";
    [@bs.module "react-apollo"] external queryComponent : ReasonReact.reactClass = "Query";
    
    type response =
      | Loading
      | Error(apolloError)
      | Data(Config.t);

    type renderPropObj = {
      result: response,
      data: option(Config.t),
      error: option(apolloError),
      loading: bool,
      refetch: option(Js.Json.t) => Js.Promise.t(response),
      fetchMore: (~variables: Js.Json.t) => Js.Promise.t(unit),
      networkStatus: int,
    };

    type renderPropObjJS = {
      .
      "loading": Js.boolean,
      "data": Js.Nullable.t(Js.Json.t),
      "error": Js.Nullable.t(apolloError),
      "refetch":
        [@bs.meth] (
          Js.Null_undefined.t(Js.Json.t) => Js.Promise.t(renderPropObjJS)
        ),
      "networkStatus": int,
      "variables": Js.Null_undefined.t(Js.Json.t),
      "fetchMore": [@bs.meth] (apolloOptions => Js.Promise.t(unit)),
    };

    let graphqlQueryAST = [@bs] gql(Config.query);

    let apolloDataToVariant: renderPropObjJS => response =
    apolloData =>
      switch (
        apolloData##loading |> Js.to_bool,
        apolloData##data |> Js.Nullable.to_opt,
        apolloData##error |> Js.Nullable.to_opt,
      ) {
      | (true, _, _) => Loading
      | (false, Some(response), _) => Data(Config.parse(response))
      | (false, _, Some(error)) => Error(error)
      | (false, None, None) => Error({"message": "No data", "graphQLErrors": Js.Nullable.null, "networkError": Js.Nullable.null})
      };
    
    let convertJsInputToReason = (apolloData: renderPropObjJS) => {
      result: apolloData |> apolloDataToVariant,
      data: switch (apolloData##data |> ReasonApolloUtils.getNonEmptyObj) {
      | None => None
      | Some(data) => 
        switch (Config.parse(data)) {
        | parsedData => Some(parsedData)
        | exception _ => None
        }
      },
      error: switch (apolloData##error |> Js.Nullable.to_opt) {
      | Some(error) => Some(error)
      | None => None
      },
      loading: apolloData##loading |> Js.to_bool,
      refetch: variables =>
        apolloData##refetch(variables |> Js.Nullable.from_opt)
        |> Js.Promise.then_(data => data |> apolloDataToVariant |> Js.Promise.resolve),
      fetchMore: (~variables) =>
        apolloData##fetchMore({"variables": variables, "query": graphqlQueryAST}),
      networkStatus: apolloData##networkStatus
    };

    let make = (
      ~variables: option(Js.Json.t)=?,
      ~pollInterval: option(int)=?,
      ~notifyOnNetworkStatusChange: option(bool)=?,
      ~fetchPolicy: option(string)=?,
      ~errorPolicy: option(string)=?,
      ~ssr: option(bool)=?,
      ~displayName: option(string)=?,
      ~delay: option(bool)=?,
      ~context: option(Js.Json.t)=?,
      children: renderPropObj => ReasonReact.reactElement
    ) => ReasonReact.wrapJsForReason(
      ~reactClass=queryComponent,
      ~props=Js.Nullable.(
        {
          "query": graphqlQueryAST,
          "variables": variables |> from_opt,
          "pollInterval": pollInterval |> from_opt,
          "notifyOnNetworkStatusChange": notifyOnNetworkStatusChange 
            |> Js.Option.map([@bs] (b) => Js.Boolean.to_js_boolean(b)) 
            |> from_opt,
          "fetchPolicy": fetchPolicy |> from_opt,
          "errorPolicy": errorPolicy |> from_opt,
          "ssr": ssr
            |> Js.Option.map([@bs] (b) => Js.Boolean.to_js_boolean(b)) 
            |> from_opt,
          "displayName": displayName |> from_opt,
          "delay": delay |> from_opt,
          "context": context |> from_opt
        }
      ),
      apolloData => apolloData |> convertJsInputToReason |> children
    );
  };
