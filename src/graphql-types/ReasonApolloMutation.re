
module MutationFactory = () => {
    external cast : string => {. "data": Js.Json.t, "loading": bool} = "%identity";
    [@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";
    [@bs.module] external mutationComponent: ReasonReact.reactClass = "react-apollo";

    type state =
      | NotCalled
      | Loading
      | Loaded(Js.Json.t)
      | Failed(Js.Promise.error);

    let make = children => ReasonReact.wrapJsForReason(
      ~reactClass=mutationComponent,
      ~props=Js.Obj.empty(),
      children
    );
};
