open ReasonApolloTypes;

type queryObj = Js.t {. query: queryString};
type generatedApolloClient = Js.t {. query: (queryObj => string)[@bs.meth]};
type generatedNetworkInterface;
type networkInterface = Js.t {. uri: string};
type apolloClient = Js.t {. networkInterface: generatedNetworkInterface};

external createNetworkInterface: networkInterface => generatedNetworkInterface = "createNetworkInterface" [@@bs.module "react-apollo"];
external createApolloClient: apolloClient => generatedApolloClient = "ApolloClient" [@@bs.new] [@@bs.module "react-apollo"];

module type CreationConfig = {
  let uri: string;
};

module type ClientConfig = {
  type responseType;
  let query: ReasonApolloTypes.queryString;
};

module Create = fun (CreationConfig:CreationConfig) => fun (ClientConfig:ClientConfig) => {
  external cast: string => Js.t {. data: ClientConfig.responseType, loading: bool } = "%identity";

  type action =
    | Result string
    | Error string;

  type state = {
    result: Js.t {. data: string, loading: bool},
    error: string
  };

  let networkInterface = createNetworkInterface {"uri": CreationConfig.uri};
  let apolloClient = createApolloClient { "networkInterface": networkInterface };

  let component = ReasonReact.reducerComponent "ReasonApollo";

  let make children => {
      ...component,
      initialState: fun () => {
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
      didMount: fun {reduce} => {
        let _ = Js.Promise.(
          resolve (apolloClient##query {"query": ClientConfig.query})
          |> then_ (fun value => {
              reduce (fun () => Result value) ();
              resolve ();
            })
          |> catch (fun _value => {
             reduce (fun () => Error "an error happened") ();
             resolve ();
           })
        );
        ReasonReact.NoUpdate;
      },
      render: fun {state} => {
        let result = {
          "loading": state.result##loading,
          "data": (cast state.result##data)##data
        };
        children.(0) result;
      }
    };
};
