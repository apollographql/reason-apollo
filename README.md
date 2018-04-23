# Reason-apollo

[![npm version](https://badge.fury.io/js/reason-apollo.svg)](https://badge.fury.io/js/reason-apollo)
[![Get on Slack](https://img.shields.io/badge/slack-join-orange.svg)](http://www.apollostack.com/#slack)

Easily use the Apollo Client 2 with Reason

## Install and setup

#### Install
```
yarn add reason-apollo

# Add graphql_ppx
yarn add --dev graphql_ppx

# Add JS dependencies
yarn add react-apollo apollo-client apollo-cache-inmemory apollo-link apollo-link-context apollo-link-error apollo-link-http graphql graphql-tag
```

#### bsconfig
Add `reason-apollo` to your `bs-dependencies` and
`graphql_ppx/ppx` to your `ppx_flags`

**bsconfig.json**
```
"bs-dependencies": [
  "reason-react",
  "reason-apollo"
],
"ppx-flags": [
    "graphql_ppx/ppx"
]
```

#### send introspection query
This will generate a `graphql_schema.json` which will be used to safely type your GraphQL queries/mutations.
```
yarn send-introspection-query http://my-api.example.com/api
```

## Why reason-apollo?
Watch it's usage in this video:  

[![Watch reason-apollo usage here](https://i.ytimg.com/vi/yMqE37LqRLA/hqdefault.jpg?sqp=-oaymwEZCNACELwBSFXyq4qpAwsIARUAAIhCGAFwAQ==&rs=AOn4CLD9rxIyXtckkxmGAxRn_Uv2mDcXcQ)](https://www.youtube.com/watch?v=yMqE37LqRLA)

# Usage  
 
 ## Create the Apollo Client
 
 **Client.re**
 ```reason

 /* Create an InMemoryCache */
 let inMemoryCache = ApolloInMemoryCache.createInMemoryCache(());

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(~uri="http://localhost:3010/graphql", ());

let instance = ReasonApollo.createApolloClient(
  ~link=httpLink, 
  ~cache=inMemoryCache, 
  ()
);

 ```
  
  ## ApolloProvider

  ***Index.re***
  ```reason

    /* 
      Enhance your application with the `ReasonApollo.Provider` 
      passing him your client instance 
    */
    ReactDOMRe.renderToElementWithId(
      <ReasonApollo.Provider client=Client.instance>
          <App />
      </ReasonApollo.Provider>
    , "index");

  ```

  ## Query
  
  **MyComponent.re**
  ```reason
  /* Create a GraphQL Query by using the graphql_ppx */ 
  module GetPokemon = [%graphql {|
    query getPokemon($name: String!){
        pokemon(name: $name) {
            name
        }
    }
  |}]; 

  module GetPokemonQuery = ReasonApollo.CreateQuery(GetPokemon);

  let make = (_children) => {
  /* ... */,
  render: (_) => {
    let pokemonQuery = PokemonQuery.make(~name="Pikachu", ());
    <GetPokemonQuery variables=pokemonQuery##variables>
      ...(({result}) => {
        switch result {
           | NoData => <div> (Utils.ste("No Data")) </div>
           | Loading => <div> (Utils.ste("Loading")) </div>
           | Error(error) => <div> (Utils.ste(error)) </div>
           | Data(response) => <div> (Utils.ste(response##pokemon##name)) </div>
        }
      })
    </GetPokemonQuery>
  }
  }
  ```

  ## Mutation
  
  **MyMutation.re**
  ```reason
  module AddPokemon = [%graphql {|
    mutation addPokemon($name: String!) {
        addPokemon(name: $name) {
            name
        }
    }
  |}];

  module AddPokemonMutation = ReasonApollo.CreateMutation(AddPokemon);

  let make = (_children) => {
  /* ... */,
  render: (_) => {  
    <AddPokemonMutation>
      ...((
        mutation /* Mutation to call */, 
        _ /* Result of your mutation */
      ) => {
        let dic = Js.Dict.empty();
        Js.Dict.set(dic, "name", Js.Json.string("Bob"));
        let newPokemon = Js.Json.object_(dic);

        <div>
          <button onClick=((_mouseEvent) => {
              mutation({
                  "variables": Js.Nullable.return(newPokemon),
                  "refetchQueries": [|"getAllPokemons"|]
              }) |> ignore;
            })> 
            (Utils.ste("Add Pokemon")) 
          </button>
        </div>
      })
    </AddPokemonMutation>
  }
  }
  ```

## ApolloConsumer

If you simply wan't to have access to the ApolloClient, you can use the `ApolloConsumer`

```reason
<ApolloConsumer>
  ...((apolloClient) => {
    /* We have access to the client! */
  })
</ApolloConsumer>
```