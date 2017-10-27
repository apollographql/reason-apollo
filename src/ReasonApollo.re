open ReasonApolloTypes;

type queryObj = Js.t {. query: queryString};
type generatedApolloClient = Js.t {. query: queryObj => string};

type state = {
  client : generatedApolloClient,
  result: Js.t {. data: string, loading: bool},
  error: string
};

type action =
  | Result string
  | Error string;

type generatedNetworkInterface;
type networkInterface = Js.t {. uri: string};
type apolloClient = Js.t {. networkInterface: generatedNetworkInterface};

external createNetworkInterface: networkInterface => generatedNetworkInterface = "createNetworkInterface" [@@bs.module "react-apollo"];
external createApolloClient: apolloClient => generatedApolloClient = "ApolloClient" [@@bs.new] [@@bs.module "react-apollo"];

let create uri::(uri:string) => {
  let networkInterface = createNetworkInterface {"uri": uri};
  let apolloClient = createApolloClient { "networkInterface": networkInterface };
  apolloClient;
};

let component = ReasonReact.reducerComponent "Component";

let create uri::(uri:string) => {
  let customMake query::queryString children => {
    ...component,
    initialState: fun () => {
      client: (create uri::uri),
      result: {
        "data": "",
        "loading": true
      },
      error: ""
    },
    reducer: fun action state =>
      switch action {
        | Result result => ReasonReact.Update {...state, result: { "loading": false, "data": result}}
        | Error error => ReasonReact.Update {...state, error}
      },
    didMount: fun {reduce, state} => {
      let query = state.client##query;
      let _ = Js.Promise.(
        resolve (query {"query": queryString})
        |> then_ (fun value => {
            reduce (fun () => Result value) ();
            resolve ();
          })
        |> catch (fun value => {
           reduce (fun () => Error "an error happened") ();
           resolve ();
         })
      );
      ReasonReact.NoUpdate;
    },
    render: fun {state} => {
      children.(0) state.result;
    }
  };
  customMake;
};