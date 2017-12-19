module type InternalConfig = {let apolloClient: ApolloClient.generatedApolloClient;};
module type ClientConfig = {type response; type variables; let mutation: ReasonApolloTypes.queryString;};

module MutationFactory = (InternalConfig:InternalConfig) => (ClientConfig: ClientConfig) => {
    module CastApolloClient = ApolloClient.Cast({type variables = ClientConfig.variables});
    let apolloClient = CastApolloClient.castClient(InternalConfig.apolloClient);

    external cast : string => {. "data": ClientConfig.response, "loading": bool} = "%identity";
    type state =
      | NotCalled
      | Loading
      | Loaded(ClientConfig.response)
      | Failed(string);

    type action =
      | Result(string)
      | Error(string);

    let sendMutation = (~mutation, ~variables, ~reduce) => {
      let mutationConfig =
        switch variables {
          | Some(variables) =>
            CastApolloClient.getJSMutationConfig(~mutation=mutation, ~variables=variables, ())
          | None => CastApolloClient.getJSMutationConfig(~mutation=mutation, ())
        };

      let _ =
      Js.Promise.(
        resolve(apolloClient##mutate(mutationConfig))
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

    let component = ReasonReact.reducerComponent("ReasonApollo");
    let make = (children) => {
      ...component,
      initialState: () => NotCalled,
      reducer: (action, _state) =>
        switch action {
          | Result(result) => {
            let typedResult = cast(result)##data;
            ReasonReact.Update(Loaded(typedResult))
          }
          | Error(error) => ReasonReact.Update(Failed(error))
        },
      render: ({reduce, state}) => {
        let mutation = (~variables=?, ()) => {
          sendMutation(~mutation=ClientConfig.mutation, ~variables, ~reduce);
        };
        children[0](mutation, state);
      }
    };
  };