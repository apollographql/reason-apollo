[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

[@bs.module]
external shallowEqual : (Js.Json.t, Js.Json.t) => bool =
  "fbjs/lib/shallowEqual";

module type InternalConfig = {
  let apolloClient: ApolloClient.generatedApolloClient;
};

module QueryFactory = (InternalConfig: InternalConfig) => {
  exception InvalidState;
  type response =
    | Loading
    | Loaded(Js.Json.t)
    | Failed(string);
  type state = {
    response,
    queryObservable: ref(ApolloClient.watchQueryObservable),
    subscription: ref(option(ApolloClient.subscription))
  };
  type action =
    | UpdateResponse(response);
  type retainedProps = {variables: Js.Json.t};
  let component =
    ReasonReact.reducerComponentWithRetainedProps("ReasonApollo");
  let make = (~query as q, children) => {
    let convertResponse = response =>
      switch (
        Js.to_bool(response##loading),
        Js.Nullable.to_opt(response##data),
        Js.Nullable.to_opt(response##error)
      ) {
      | (true, _, None) => Loading
      | (false, _, Some(error)) => Failed(error)
      | (false, Some(data), None) => Loaded(data)
      | _ => raise(InvalidState) /* Should not happen */
      };
    let makeQueryObservable = (~query) =>
      InternalConfig.apolloClient##watchQuery({
        "query": [@bs] gql(query##query),
        "variables": query##variables
      });
    let startSubscription = self => {
      let queryObservable = self.ReasonReact.state.queryObservable^;
      self.state.subscription :=
        Some(
          queryObservable##subscribe({
            "next": response =>
              self.send(UpdateResponse(convertResponse(response))),
            "error": error =>
              self.send(
                UpdateResponse(
                  Failed(Js.Exn.message(error) |> Js.Option.getWithDefault(""))
                )
              )
          })
        );
      ();
    };
    let removeSubscription = self => {
      switch self.ReasonReact.state.subscription^ {
      | Some(subscription) => subscription##unsubscribe()
      | None => ()
      };
      self.state.subscription := None;
    };
    {
      ...component,
      initialState: () => {
        let queryObservable = makeQueryObservable(~query=q);
        let response = convertResponse(queryObservable##currentResult());
        {
          response,
          queryObservable: ref(queryObservable),
          subscription: ref(None)
        };
      },
      retainedProps: {
        variables: q##variables
      },
      reducer: (action, state) =>
        switch action {
        | UpdateResponse(response) => Update({...state, response})
        },
      didMount: self => {
        startSubscription(self);
        ReasonReact.NoUpdate;
      },
      willReceiveProps: self =>
        if (! shallowEqual(q##variables, self.retainedProps.variables)) {
          removeSubscription(self);
          let queryObservable = makeQueryObservable(~query=q);
          self.state.queryObservable := queryObservable;
          startSubscription(self);
          let response = convertResponse(queryObservable##currentResult());
          {...self.state, response};
        } else {
          self.state;
        },
      willUnmount: self => removeSubscription(self),
      render: ({state}) => children(state.response, q##parse)
    };
  };
};
