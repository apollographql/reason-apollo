## Reason-apollo

Easily use the Apollo client with ReasonML

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
 type getUser = {. name: string};
 type data = {. getUser: getUser};
 ```
 
 ##### All in a module
 data structure of the response and the query should be represented in a module 
 ```
 module Config = {
   type responseType = data;
   let query = query;
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
 <FetchUserName>
   ((response) => {
     /* The response of your query is available here */
   })
 </FetchUserName>
 ```
 
 [here](https://github.com/Gregoirevda/reason-apollo-test-usage) is a repository showing the usage of the package
 
 
 ### Todo
 If you think this is too verbose, I have good news for you:   
 Using Apollo Client in Reason will be much easier and less verbose compared to ReactJS.  
 More to come in the next weeks.