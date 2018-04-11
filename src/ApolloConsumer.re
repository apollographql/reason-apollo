open ApolloClient;

[@bs.module "react-apollo"]
  external apolloConsumer : ReasonReact.reactClass = "ApolloConsumer";
  
  let make = (children: generatedApolloClient => ReasonReact.reactElement) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=apolloConsumer, 
      ~props=Js.Obj.empty(), 
      children
    );