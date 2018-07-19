/* Create an InMemoryCache */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache(());

/* Create an HTTP Link */
let httpLink =
 ApolloLinks.createHttpLink(~uri="https://boiling-bastion-96890.herokuapp.com/graphql", ());


/* WebSocket client */
let webSocketLink = ApolloLinks.webSocketLink(~uri="wss://boiling-bastion-96890.herokuapp.com/graphql", ~reconnect=true, ());

/* based on test, execute left or right */
let webSocketHttpLink = ApolloLinks.split(
  (operation) => {
    let operationDefition = ApolloUtilities.getMainDefinition(operation##query);
    operationDefition##kind == "OperationDefinition" && operationDefition##operation == "subscription";
  },
  webSocketLink,
  httpLink,
);

let instance = ReasonApollo.createApolloClient(~link=webSocketHttpLink, ~cache=inMemoryCache, ());

