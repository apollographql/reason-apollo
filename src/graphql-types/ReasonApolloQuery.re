external castResponse : string => {. "data": Js.Json.t} = "%identity";

external asJsObject : 'a => Js.t({..}) = "%identity";

[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

[@bs.module] external shallowEqual : (Js.t({..}), Js.t({..})) => bool = "fbjs/lib/shallowEqual";

type response =
  | Loading
  | Loaded(Js.Json.t)
  | Failed(string);

type state = {
  response,
  variables: Js.Json.t
};

type action =
  | Result(string)
  | Error(string);

let sendQuery = (~client, ~query, ~reduce) => {
  let _ =
    Js.Promise.(
      resolve(client##query({"query": [@bs] gql(query##query), "variables": query##variables}))
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
  ()
};

let component = ReasonReact.reducerComponent("ReasonApollo");

let make = (~client, ~query as q, children) => {
  ...component,
  initialState: () => {response: Loading, variables: q##variables},
  reducer: (action, state) =>
    switch action {
    | Result(result) =>
      let typedResult = castResponse(result)##data;
      ReasonReact.Update({...state, response: Loaded(typedResult)})
    | Error(error) => ReasonReact.Update({...state, response: Failed(error)})
    },
  willReceiveProps: ({state, reduce}) =>
    if (! shallowEqual(asJsObject(q##variables), asJsObject(state.variables))) {
      sendQuery(~client, ~query=q, ~reduce);
      state
    } else {
      state
    },
  didMount: ({reduce}) => {
    sendQuery(~client, ~query=q, ~reduce);
    ReasonReact.NoUpdate
  },
  render: ({state}) => children(state.response, q##parse)
};