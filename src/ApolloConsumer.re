open ApolloClient;

[@bs.module "react-apollo"] [@react.component]
external make: (~children: generatedApolloClient) => React.element =
  "ApolloConsumer";
