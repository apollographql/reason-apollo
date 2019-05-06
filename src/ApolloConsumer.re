open ApolloClient;

module JsConsumer = {
  [@bs.module "react-apollo"] [@react.component]
  external make:
    (~children: generatedApolloClient => React.element) => React.element =
    "ApolloConsumer";
};

[@react.component]
let make = (~children: generatedApolloClient => React.element) =>
  <JsConsumer> {client => children(client)} </JsConsumer>;
