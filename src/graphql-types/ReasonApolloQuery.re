module type InternalConfig = {let apolloClient: ApolloClient.generatedApolloClient;};

module QueryFactory = (InternalConfig:InternalConfig) => {
    external castResponse : string => {. "data": Js.Json.t } = "%identity";            

    type response =
      | Loading
      | Loaded(Js.Json.t)
      | Failed(string);

    type state = { 
      response: response, 
      variables: Js.Json.t
    };

    type action =
      | Result(string)
      | Error(string);

    let sendQuery = (~query, ~reduce) => {
      let _ =
      Js.Promise.(
        resolve(InternalConfig.apolloClient##query({
          "query": query##query,
          "variables": query##variables
        }))
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
    let make = (~query, children) => {
      ...component,
      initialState: () => {
        response: Loading,
        variables: query()##variables
      },
      reducer: (action, state) =>
        switch action {
          | Result(result) => {
            let typedResult = castResponse(result)##data;
            ReasonReact.Update({
              ...state,
              response: Loaded(typedResult)
            })
          }
          | Error(error) => ReasonReact.Update({
            ...state,
            response: Failed(error)
          })
        },
      willReceiveProps: ({state, reduce}) => {
        if(query()##variables !== state.variables) {
          sendQuery(~query=query(), ~reduce);
          state;
        } else {
          state;
        }
      },
      didMount: ({reduce}) => {
        sendQuery(~query=query(), ~reduce);
        ReasonReact.NoUpdate;
      },
      render: ({state}) => {
        children[0](state.response, query()##parse);
      }
    };
  };