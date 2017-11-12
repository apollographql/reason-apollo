module type CreationConfig = {let uri: string;};

module type ClientConfig = {type responseType;};

module Create = (CreationConfig: CreationConfig, ClientConfig: ClientConfig) => {
  external cast : string => {. "data": ClientConfig.responseType, "loading": bool} = "%identity";
  type action =
    | Result(string)
    | Error(string);
  type state = {
    result: {. "data": string, "loading": bool},
    error: string
  };
  let httpLinkOptions: ApolloClient.linkOptions = {"uri": CreationConfig.uri};
  let apolloClientOptions: ApolloClient.clientOptions = {
    "cache": ApolloClient.inMemoryCache(),
    "link": ApolloClient.httpLink(httpLinkOptions)
  };
  let apolloClient = ApolloClient.apolloClient(apolloClientOptions);
  let component = ReasonReact.reducerComponent("ReasonApollo");
  let make = (~query=?, children) => {
    ...component,
    initialState: () => {result: {"data": "", "loading": true}, error: ""},
    reducer: (action, state) =>
      switch action {
      | Result(result) =>
        ReasonReact.Update({...state, result: {"loading": false, "data": result}})
      | Error(error) => ReasonReact.Update({...state, error})
      },
    didMount: ({reduce}) => {
      switch query {
        | Some query => {
          let _ =
            Js.Promise.(
              resolve(apolloClient##query({"query": query}))
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
        }
      };
      ReasonReact.NoUpdate
    },
    render: ({state}) => {
      let result = {"loading": state.result##loading, "data": cast(state.result##data)##data};
      children[0](result)
    }
  };
};