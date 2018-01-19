open ApolloLinks;

open ApolloInMemoryCache;

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
let errorHandler = errorResponse =>
  switch errorResponse##networkError {
  | Some(error) =>
    if (error##statusCode == 401) {
      logout();
    } else {
      ();
    }
  | None => ()
};

let errorLink = createErrorLink(errorHandler);

/* Create an InMemoryCache */
module InMemoryCache =
  CreateInMemoryCache(
    {
      type dataObject = {. "__typename": string, "id": string, "key": string};
      let inMemoryCacheObject =
        Js_null_undefined.return({
          "dataIdFromObject": (obj: dataObject) =>
            if (obj##__typename === "Organization") {
              obj##key;
            } else {
              obj##id;
            }
        });
    }
  );

/* Alternatively if you want to use the default values of InMemoryCache: */
/*
 module InMemoryCache =
   CreateInMemoryCache(
     {
       type dataObject;
       let inMemoryCacheObject = Js_null_undefined.undefined;
     }
   );
 */


/* Create the ApolloClient */
module Instance =
  ReasonApollo.CreateClient(
    {
      let apolloClient =
        ReasonApollo.createApolloClient(
          ~cache=InMemoryCache.cache,
          ~link=from([|authLink, errorLink, httpLink|]),
          ()
        );
    }
  );