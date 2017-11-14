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
 ```
 module Client = ReasonApollo.Create({ let uri = "http://localhost:3010/graphql"});
 
 ```
 
  
  ##### Fetching data
  
  ###### Query
  Create a query with the `graphql-tag`
  ```
  let query =
    gql {|
      query getUser {
        name
      }
    |} [@bs];
  ```
  ##### Defining the data structure of the result
  ```
  type user = {. "name": string};
  type data = {. "user": user};
  ```
  
  ##### Optional variables passed to the query
  ```
  let variables = {
    "limit": 2
  };
  ```
  
  ##### All in a module
  data structure of the response and optional variables should be represented in a module 
  ```
  module Config = {
    type responseType = data;
    type variables = {. limit: int}; /* or `type variables;` if none are used */
  };
 
  ```
  
  ##### Passing the configuration to the Apollo Client
  ```
  module FetchUserName = Apollo.Client(Config);
  ```
  
  ##### Executing the query
  someFile.re
  ```
  render: (_) =>
  <FetchUserName query variables>
    ((response) => {
      /* The response of your query is available here */
    })
  </FetchUserName>
  ```
