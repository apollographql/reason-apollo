module type InternalConfig = {let apolloClient: ApolloClient.generatedApolloClient;};

module QueryFactory = (InternalConfig: InternalConfig) => {
  let component = ReasonReact.statelessComponent("Query");
  let make = (~query, children) => {
    ...component,
    render: (_self) =>
      <ReasonApolloQuery query client=InternalConfig.apolloClient> ...children </ReasonApolloQuery>
  };
};

module MutationFactory = (InternalConfig: InternalConfig) => {
  let component = ReasonReact.statelessComponent("Mutation");
  let make = (children) => {
    ...component,
    render: (_self) =>
      <ReasonApolloMutation client=InternalConfig.apolloClient> ...children </ReasonApolloMutation>
  };
};