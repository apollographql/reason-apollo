/* open ApolloLinks; */
open ApolloInMemoryCache;

type dataObject = {
  .
  "__typename": string,
  "id": string
};

/* Create an InMemoryCache */
let inMemoryCache =
  createInMemoryCache(~dataIdFromObject=(obj: dataObject) => obj##id, ());

/* Create an HTTP Link */
let httpLink = ApolloLinks.createHttpLink(~uri="https://api.graph.cool/simple/v1/cjdgba1jw4ggk0185ig4bhpsn", ());

let instance = ReasonApollo.createApolloClient(~link=httpLink, ~cache=inMemoryCache, ());
