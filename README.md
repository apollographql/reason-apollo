# Reason-apollo

[![npm version](https://badge.fury.io/js/reason-apollo.svg)](https://badge.fury.io/js/reason-apollo)
[![Get on Slack](https://img.shields.io/badge/slack-join-orange.svg)](http://www.apollostack.com/#slack)

React-apollo with Reason

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
      passing it your client instance 
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
    let pokemonQuery = GetPokemon.make(~name="Pikachu", ());
    <GetPokemonQuery variables=pokemonQuery##variables>
      ...(({result}) => {
        switch result {
           | Loading => <div> (ReasonReact.string("Loading")) </div>
           | Error(error) => <div> (ReasonReact.string(error)) </div>
           | Data(response) => <div> (ReasonReact.string(response##pokemon##name)) </div>
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
        let newPokemon = AddPokemon.make(~name="Bob", ());
        <div>
          <button onClick=((_mouseEvent) => {
              mutation(
                 ~variables=newPokemon##variables, 
                 ~refetchQueries=[|"getAllPokemons"|], 
                 ()
              ) |> ignore;
            })> 
            (ReasonReact.string("Add Pokemon")) 
          </button>
        </div>
      })
    </AddPokemonMutation>
  }
  }
  ```

## ApolloConsumer

If you simply want to have access to the ApolloClient, you can use the `ApolloConsumer`

```reason
<ApolloConsumer>
  ...((apolloClient) => {
    /* We have access to the client! */
  })
</ApolloConsumer>
```
## FAQ

### I've added the schema file, but my build fails saying it couldn't be found

In some cases, it seems like there are some differences between the provided `send-introspection-query`
and output from tools you might be using to download the schema (such as `apollo-codegen` or `graphql-cli`).
If your build is failing, please make sure to try with the provided script. In your project root, run:

```bash
$ yarn send-introspection-query <url>
```
### My response includes a field that starts with an upper case letter

At this time, reason object field names need to start lowercase. Therefore if you have a request like this:
```
{
    Link {
      id
      title
    }
}
```

You will try and access the response object this way but it will throw an error:

```
response##Link
```

Instead, use an alias to modify the response:
```
{
    link: Link {
      id
      title
    }
}
```

Then you can access the object like this:
```
response##link
```
