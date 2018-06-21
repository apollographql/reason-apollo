/* open ApolloLinks; */
open ApolloInMemoryCache;

type dataObject = {
  .
  "__typename": string,
  "id": string
};

/* Create an InMemoryCache */
let inMemoryCache =
  createInMemoryCache(());

/* Create an HTTP Link */
let httpLink = ApolloLinks.createHttpLink(~uri="https://api.graph.cool/simple/v1/cjdgba1jw4ggk0185ig4bhpsn", ());

/* WebSocket client */
/*let subscriptionClient = ApolloLinks.subscriptionClient(~uri="wss://subscriptions.graph.cool/v1/cjdgba1jw4ggk0185ig4bhpsn", ());*/


type options = {
  .
  "reconnect": bool
};

type opt = {
  .
  "uri": string,
  "options": options
};

let options = {
  "reconnect": true
};

let opt = { "uri": "wss://subscriptions.graph.cool/v1/cjdgba1jw4ggk0185ig4bhpsn", "options": options  };

let webSocketLink = ApolloLinks.webSocketLink(opt);
Js.log2("wslink", webSocketLink);

let webSocketHttpLink = ApolloLinks.split(
  (operation) => {
    let operationDefition = ApolloLinks.getMainDefinition(operation##query);
    operationDefition##kind == "OperationDefinition" && operationDefition##operation == "subscription";
  },
  webSocketLink,
  httpLink,
);

let instance = ReasonApollo.createApolloClient(~link=webSocketHttpLink, ~cache=inMemoryCache, ());
