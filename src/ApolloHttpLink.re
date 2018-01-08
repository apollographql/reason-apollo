open ReasonApolloTypes;

/**
 * A convenience method to cast a generic type to a record object.
 */
external asJsObject : 'a => Js.t({..}) =
  "%identity";

/**
 * CreateHttpLink
 * https://github.com/apollographql/apollo-link/tree/master/packages/apollo-link-http
 */
module type HttpLinkConfig = {
  /**
   * The endpoint where the GraphQL server is running.
   * https://github.com/apollographql/apollo-link/tree/master/packages/apollo-link-http#options
   * TODO: support other options
   */
  let uri: string;
};

module CreateHttpLink = (Config: HttpLinkConfig) => {
  /* Bind the HttpLink class */
  [@bs.module "apollo-link-http"] [@bs.new]
  external httpLink : ApolloClient.linkOptions => apolloLink =
    "HttpLink";
  /* Instanciate a new http link object */
  let link = httpLink({"uri": Config.uri});
};