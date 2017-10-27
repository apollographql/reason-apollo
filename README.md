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

Create a new file:

Apollo.re
```
let make = ReasonApollo.create uri::"http://localhost:3010/graphql";

```

The name of the file will be the name of your ApolloClient module.  
You need to assign it to a `make` variable. (JSX calls this function under the hood)
 
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
 
 ##### Executing the query
 someFile.re
 ```
 render: fun _ =>
 <Apollo query>
   (fun response => {
     /* The response of your query is available here */
   })
 </Apollo>
 ```