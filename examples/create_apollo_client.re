open ApolloLinks;

open ApolloInMemoryCache;

/* Fake some methods, for the sake of the example */
let getAccessToken = () => "123";

let logout = () => ();

/* Create an HTTP Link */
module HttpLink =
  CreateHttpLink(
    {
      let uri = "http://localhost:3010/graphql";
    }
  );

/* Create a Link that puts an Authorization header in context */
module AuthLink =
  CreateContextLink(
    {
      let contextHandler = () => {
        let token = getAccessToken();
        let headers = {"headers": {"authorization": {j|Bearer $token|j}}};
        asJsObject(headers)
      };
    }
  );

/* Create a Link that handles 401 error responses */
module ErrorLink =
  CreateErrorLink(
    {
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
    }
  );

/* Create an InMemoryCache */
module InMemoryCache =
  CreateInMemoryCache(
    {
      type dataObject = {. "__typename": string, "id": string, "key": string};
      let inMemoryCacheObject =
        Js_null_undefined.return({
          "dataIdFromObject": (obj: dataObject) =>
            if (obj##__typename === "Organization") {
              obj##key
            } else {
              obj##id
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
let createApolloClient =
  InitApolloClient.createClient(
    ~ssrMode=true,
    ~cache=InMemoryCache.cache,
    ~link=from([|AuthLink.link, ErrorLink.link, HttpLink.link|]),
    ()
  );