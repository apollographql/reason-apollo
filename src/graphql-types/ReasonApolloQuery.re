module type InternalConfig = {let apolloClient: ApolloClient.generatedApolloClient;};
module type ClientConfig = {type response; type variables; let query: ReasonApolloTypes.queryString;};

module QueryFactory = (InternalConfig:InternalConfig) => (ClientConfig: ClientConfig) => {
    module CastApolloClient = ApolloClient.Cast({type variables = ClientConfig.variables});
    let apolloClient = CastApolloClient.castClient(InternalConfig.apolloClient);

    external cast : string => {. "data": ClientConfig.response, "loading": bool} = "%identity";
    type state =
      | Loading
      | Loaded(ClientConfig.response)
      | Failed(string);

    type action =
      | Result(string)
      | Error(string);

    type retainedProps = {variables: option(ClientConfig.variables)};

    let sendQuery = (~query, ~variables, ~reduce) => {
      let queryConfig =
        switch variables {
          | Some(variables) =>
            CastApolloClient.getJSQueryConfig(~query=query, ~variables=variables, ())
          | None => CastApolloClient.getJSQueryConfig(~query=query, ())
        };
      let _ =
      Js.Promise.(
        resolve(apolloClient##query(queryConfig))
        |> then_(
             (value) => {
               reduce(() => Result(value), ());
               resolve()
             }
           )
        |> catch(
             (_value) => {
               reduce(() => Error("an error happened"), ());
               resolve()
             }
           )
      );
    };

    let component = ReasonReact.reducerComponentWithRetainedProps("ReasonApollo");
    let make = (~variables=?, children) => {
      ...component,
      initialState: () => Loading,
      reducer: (action, _state) =>
        switch action {
          | Result(result) => {
            let typedResult = cast(result)##data;
            ReasonReact.Update(Loaded(typedResult))
          }
          | Error(error) => ReasonReact.Update(Failed(error))
        },
      retainedProps: {variables: variables},
      willReceiveProps: ({retainedProps, state, reduce}) => {
        switch (variables, retainedProps.variables) {
          | (Some(_variables), Some(retainedVariables)) => {
            if(_variables !== retainedVariables) {
              sendQuery(~query=ClientConfig.query, ~variables, ~reduce);
              state;
            } else {
              state;
            }
          }
          | _ => state
        }
      },
      didMount: ({reduce}) => {
        sendQuery(~query=ClientConfig.query, ~variables, ~reduce);
        ReasonReact.NoUpdate;
      },
      render: ({state}) => {
        children[0](state);
      }
    };
  };