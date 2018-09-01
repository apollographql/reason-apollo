/* open ApolloLinks; */
open ApolloInMemoryCache;

/* Create an InMemoryCache */
let inMemoryCache =
  createInMemoryCache(());

/*
  OR with dataIdFromObject:

  type dataObject = {
    .
    "__typename": string,
    "id": string
  };

  createInMemoryCache(~dataIdFromObject=(obj: dataObject) => obj##id, ());
*/

/* Create a Link that puts an Authorization header in context */	
let headerContextLink = ApolloLinks.createContextLink(() => {
  "headers": {"authorization": "Bearer $123"}
});

/* Create an HTTP Link */
let httpLink = ApolloLinks.createHttpLink(~uri="https://api.graph.cool/simple/v1/cjdgba1jw4ggk0185ig4bhpsn", ());

/* WebSocket client */
let webSocketLink = ApolloLinks.webSocketLink(~uri="ws://localhost:8080/graphql", ~reconnect=true, ());

/* based on test, execute left or right */
let webSocketHttpLink = ApolloLinks.split(
  (operation) => {
    let operationDefition = ApolloUtilities.getMainDefinition(operation##query);
    operationDefition##kind == "OperationDefinition" && operationDefition##operation == "subscription";
  },
  webSocketLink,
  httpLink,
);

let instance = ReasonApollo.createApolloClient(~link=ApolloLinks.from([|webSocketHttpLink, headerContextLink|]), ~cache=inMemoryCache, ());
