type operationDefinitionNode = {
  kind: string,
  operation: string,
};

[@bs.module "apollo-utilities"]
external getMainDefinition:
  ReasonApolloTypes.documentNodeT => operationDefinitionNode =
  "getMainDefinition";
