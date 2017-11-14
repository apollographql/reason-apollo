module type CreationConfig = {let uri: string;};

module type ClientConfig = {type responseType; type variables;};

module Create = (CreationConfig: CreationConfig, ClientConfig: ClientConfig) => {
  external cast : string => {. "data": ClientConfig.responseType, "loading": bool} = "%identity";
  type state =
    | Loading
    | Loaded(ClientConfig.responseType)
    | Failed(string);

  type action =
    | Result(string)
    | Error(string);

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
    initialState: () => Loading,
    reducer: (action, state) =>
      switch action {
        | Result(result) => {
          let typedResult = cast(result)##data;
          ReasonReact.Update(Loaded(typedResult))
        }
        | Error(error) => ReasonReact.Update(Failed(error))
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
      children[0](state);
    }
  };
};