# Reason-apollo

[![npm version](https://badge.fury.io/js/reason-apollo.svg)](https://badge.fury.io/js/reason-apollo)
[![Get on Slack](https://img.shields.io/badge/slack-join-orange.svg)](http://www.apollostack.com/#slack)

Easily use the Apollo Client 2 with Reason

## Install and setup

#### Install
```
yarn add reason-apollo

# Add graphql_ppx
opam install graphql_ppx
yarn add --dev graphql_ppx
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
This will generate a `schema.json` which will be used to safely type your GraphQL queries/mutations.
```
yarn send-introspection-query http://my-api.example.com/api
```

Remember to run `bsb -clean-world` if run this command again.

## Usage 
 
 [here](https://github.com/Gregoirevda/reason-apollo-test-usage) is a repository showing the usage of the package.
 
 
 ### Create the Apollo Client
 
 **Apollo.re**
 ```reason
 open ApolloLinks;
 open ApolloInMemoryCache;

 module InMemoryCache =
  ApolloInMemoryCache.CreateInMemoryCache(
    {
      type dataObject;
      let inMemoryCacheObject = Js.Nullable.undefined;
    }
  );

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(~uri="http://localhost:3010/graphql", ());

module Client =
  ReasonApollo.CreateClient(
    {
      let apolloClient =
        ReasonApollo.createApolloClient(
          ~cache=InMemoryCache.cache,
          ~link=httpLink,
          ()
        );
    }
  );

 ```
  
  ## Query
  
  **MyComponent.re**
  ```reason
  /* Create a GraphQL Query by using the graphql_ppx */ 
  module PokemonQuery = [%graphql {|
    query getPokemon($name: String!){
        pokemon(name: $name) {
            name
        }
    }
  |}]; 

  module Query = Client.Instance.Query;

  let make = (_children) => {
  /* ... */
  render: (_) => {
    let pokemonQuery = PokemonQuery.make(~name="Pikachu", ());
    <Query query=pokemonQuery>
      (response, parse) => {
        switch response {
           | Loading => <div> (Utils.ste("Loading")) </div>
           | Failed(error) => <div> (Utils.ste(error)) </div>
           | Loaded(result) => <div> (Utils.ste(parse(result)##user##name)) </div>
      }})
    </Query>
  }
  }
  ```

  ## Mutation
  
  **MyMutation.re**
  ```reason
  module PokemonMutation = [%graphql {|
    mutation addPokemon($name: String!) {
        addPokemon(name: $name) {
            name
        }
    }
  |}];

  module Mutation = Client.Instance.Mutation;
  
  let make = (_children) => {
  /* ... */
  initialState: {
    parse
  },
  reducer: (action, state) =>
    switch (action) {
    | AddParser(parse) => ReasonReact.Update({...state, parse})
  },
  render: ({reduce, state: {parse}}) => {  
    <Mutation>
      ((
        mutate /* Mutation to call */, 
        result /* Result of your mutation */
      ) => {
          let mutationResponse = switch result {
            | NotCalled => <div>  (Utils.ste("Not Called")) </div>
            | Loading => <div> (Utils.ste("Loading")) </div>
            | Loaded(response) => <div> (Utils.ste(parse(result)##addPokemon##name ++ " addded")) </div>
            | Failed(error) => <div> (Utils.ste(error)) </div>
          };
        <div>
          <button onClick=((_mouseEvent) => {
              let pokemonMutation = PokemonMutation.make(~name="Reason", ());
              mutate(pokemonMutation);
              reduce(() => AddParser(pokemonMutation##parse), ());
            })> 
            (Utils.ste("Add Pokemon")) 
          </button>
          <div> (mutationResponse) </div>
        </div>
      })
    </Mutation>
  }
  }
  ```
