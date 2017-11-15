## Reason-apollo

Easily use the Apollo client 2 with ReasonML

#### Install

##### npm || yarn
```
yarn add reason-apollo
or
npm install reason-apollo
```

##### bsconfig
In bsconfig.json, add `reason-apollo` to your `bs-dependencies`:
```
"bs-dependencies": [
  "reason-react",
  "reason-apollo"
]
```


#### Usage 
 
 [here](https://github.com/Gregoirevda/reason-apollo-test-usage) is a repository showing the usage of the package.
 
 
 ##### Create your Client
 
 
 Apollo.re
 ```reason
 module Client = ReasonApollo.Create({ let uri = "http://localhost:3010/graphql" });

 ```
 
  
  ##### Fetching data
  
  ###### Query
  Create a query with the `graphql-tag`
  ```reason
  let query =
    gql {|
      query getUser {
        name
      }
    |} [@bs];
  ```
  ##### Defining the data structure of the result
  ```reason
  type user = {. "name": string};
  type data = {. "user": user};
  ```
  
  ##### Optional variables passed to the query
  ```reason
  let variables = {
    "limit": 2
  };
  ```
  
  ##### All in a module
  data structure of the response and optional variables should be represented in a module 
  ```reason
  module Config = {
    type responseType = data;
    type variables = {. limit: int}; /* or `type variables;` if none are used */
  };
 
  ```
  
  ##### Passing the configuration to the Apollo Client
  ```reason
  module FetchUserName = Apollo.Client(Config);
  ```
  
  ##### Executing the query
  someFile.re
  ```reason
  render: (_) =>
  <FetchUserName query variables>
    ((response) => {
      switch response {
         | Loading => <div> (Utils.ste("Loading")) </div>
         | Failed(error) => <div> (Utils.ste(error)) </div>
         | Loaded(result) =><div> (Utils.ste(result##user##name)) </div>
    })
  </FetchUserName>
  ```
