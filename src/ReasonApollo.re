module type CreationConfig = {let uri: string;};

module type ClientConfig = {type responseType; type variables;};

module Create = (CreationConfig: CreationConfig, ClientConfig: ClientConfig) => {
  external cast : string => {. "data": ClientConfig.responseType, "loading": bool} = "%identity";
  type action =
    | Result(string)
    | Error(string);
  type state = {
    result: {. "data": string, "loading": bool},
    error: string
  };
  module ConfiguredApolloClient = ApolloClient.Get({ type variables = ClientConfig.variables });
  let httpLinkOptions: ConfiguredApolloClient.linkOptions = {"uri": CreationConfig.uri};
  let apolloClientOptions: ConfiguredApolloClient.clientOptions = {
    "cache": ConfiguredApolloClient.inMemoryCache(),
    "link": ConfiguredApolloClient.httpLink(httpLinkOptions)
  };
  let apolloClient = ConfiguredApolloClient.apolloClient(apolloClientOptions);
  let component = ReasonReact.reducerComponent("ReasonApollo");
  let make = (~query, ~variables=?, children) => {
    ...component,
    initialState: () => {result: {"data": "", "loading": true}, error: ""},
    reducer: (action, state) =>
      switch action {
      | Result(result) =>
        ReasonReact.Update({...state, result: {"loading": false, "data": result}})
      | Error(error) => ReasonReact.Update({...state, error})
      },
    didMount: ({reduce}) => {
      let queryConfig =
        switch variables {
          | Some(variables) =>
            ConfiguredApolloClient.getJSQueryConfig(~query=query, ~variables=variables, ())
          | None => ConfiguredApolloClient.getJSQueryConfig(~query=query, ())
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
      ReasonReact.NoUpdate;
    },
    render: ({state}) => {
      let result = {"loading": state.result##loading, "data": cast(state.result##data)##data};
      children[0](result)
    }
  };
};