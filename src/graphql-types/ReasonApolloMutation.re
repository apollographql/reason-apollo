module type InternalConfig = {let apolloClient: ApolloClient.generatedApolloClient;};

type state =
  | NotCalled
  | Loading
  | Loaded(Js.Json.t)
  | Failed(Js.Promise.error);

type action =
  | Result(string)
  | Error(Js.Promise.error);

module type Mutation = {
  let cast: string => {. "data": Js.Json.t, "loading": bool };
  let sendMutation: (~mutation:  {. "query": string, "variables": Js.Json.t }, ~reduce: ((unit) => action, unit) => 'a) => unit;
  let component: ReasonReact.componentSpec(
    state,
    ReasonReact.stateless,
    ReasonReact.noRetainedProps,
    ReasonReact.noRetainedProps,
    action
  );
  let make: (
    ({. "query": string, "variables": Js.Json.t} => unit, state) => ReasonReact.reactElement
  ) => ReasonReact.componentSpec(
    state,
    state,
    ReasonReact.noRetainedProps,
    ReasonReact.noRetainedProps,
    action
  );
};

module MutationFactory = (InternalConfig:InternalConfig) : Mutation => {
    external cast : string => {. "data": Js.Json.t, "loading": bool} = "%identity";
    [@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";
    
    let sendMutation = (~mutation, ~reduce) => {
      let _ =
      Js.Promise.(
        resolve(InternalConfig.apolloClient##mutate({
          "mutation": [@bs] gql(mutation##query),
          "variables": mutation##variables
        }))
        |> then_(
             (value) => {
               reduce(() => Result(value), ());
               resolve()
             }
           )
        |> catch(
             (error) => {
              reduce(() => Error(error), ());
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
        let mutate = (mutationFactory) => {
          sendMutation(~mutation=mutationFactory, ~reduce);
        };
        children(mutate, state);
      }
    };
  };
