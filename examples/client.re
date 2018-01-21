open ApolloLinks;

open ApolloInMemoryCache;

/* Define type for InMemoryCache's dataIdFromObject option  */
type dataObject = {. "__typename": string, "id": string, "key": string};

/* Fake some methods, for the sake of the example */
let getAccessToken = () => "123";

let logout = () => ();

/* Create an HTTP Link */
let httpLink = createHttpLink(~uri="http://localhost:3010/graphql", ());

/* Create a Link that puts an Authorization header in context */
let contextHandler = () => {
  let token = getAccessToken();
  let headers = {"headers": {"authorization": {j|Bearer $token|j}}};
  asJsObject(headers)
};

let authLink = createContextLink(contextHandler);

/* Create a Link that handles 401 error responses */
let errorHandler = (errorResponse) =>
  switch errorResponse##networkError {
  | Some(error) =>
    if (error##statusCode == 401) {
      logout()
    } else {
      ()
    }
  | None => ()
  };

let errorLink = createErrorLink(errorHandler);

let inMemoryCache =
  createInMemoryCache(
    ~dataIdFromObject=
      (obj: dataObject) =>
        if (obj##__typename === "Organization") {
          obj##key
        } else {
          obj##id
        },
    ()
  );

/* Create the ApolloClient */
module Instance =
  ReasonApollo.CreateClient(
    {
      let apolloClient =
        ReasonApollo.createApolloClient(
          ~cache=inMemoryCache,
          ~link=from([|authLink, errorLink, httpLink|]),
          ()
        );
    }
  );