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
yarn add react-apollo apollo-client apollo-cache-inmemory apollo-link apollo-link-context apollo-link-error apollo-link-http graphql graphql-tag apollo-link-ws apollo-upload-client subscriptions-transport-ws
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
Watch its usage in this video:  

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
           | Error(error) => <div> (ReasonReact.string(error##message)) </div>
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

## Tips and Tricks

### Use `@bsRecord` on response object

The `@bsRecord` modifier is an [extension](https://github.com/mhallin/graphql_ppx#record-conversion) of the graphql syntax for BuckleScipt/ReasonML. It allows you to convert a reason object to a reason record and reap the benefits of pattern matching. For example, let's say I have a nested object of options. I would have to do something like this:

```reason
switch response##object {
| Some(object) => {
  switch object##nestedValue {
  | Some(nestedValue) => nestedValue
  | None => ""
  }
}
| None => ""
}
```

Kind of funky, huh? Let's modify the response and convert it to a reason record.

```reason
type object = {
  nestedValue: option(string)
}


module GetObject = [%graphql {|
  object @bsRecord {
    nestedValue
  }
 |}
];
```

This time we can pattern match more precisely.

```reason
switch response##object {
| Some({ nestedValue: Some(value) }) => value
| Some({ nestedValue: None }) => ""
| None => ""
}
```

### Use an alias for irregular field names

You might find yourself consuming an API with field names like `Field`. Currently, reason object field names are required to be camel case. Therefore if you have a request like this:
```reason
{
    Object {
      id
      title
    }
}
```

You will attempt to access the response object but it will throw an error:

```reason
response##Object /* Does not work :( */
```

Instead, use an `alias` to modify the response:
```reason
{
    object: Object {
      id
      title
    }
}
```

Then you can access the object like this:
```reason
response##object
```

### Generic Error and Loading components

You can create a generic error and Loading component and compose them like this example:
```reason

module QueryView = {
  let component = ReasonReact.statelessComponent(__MODULE__);

  let make =
      (
        ~result: ReasonApolloTypes.queryResponse('a),
        ~accessData: 'a => option('b),
        ~render: ('b, 'c) => ReasonReact.reactElement,
        ~onLoadMore: ('b, 'unit) => unit=(_, ()) => (),
        _children,
      ) => {
    ...component,
    render: _self =>
      switch (result) {
      | Error(error) => <Error />
      | Loading => ReasonReact.null
      | Data(response) =>
        switch (accessData(response)) {
        | Some(data) => render(data, onLoadMore(data))
        | _ => <Error error="" />
        }
      },
  };
};
```

## FAQ

### I've added the schema file, but my build fails saying it couldn't be found?

In some cases, it seems like there are some differences between the provided `send-introspection-query`
and output from tools you might be using to download the schema (such as `apollo-codegen` or `graphql-cli`).
If your build is failing, please make sure to try with the provided script. In your project root, run:

```bash
$ yarn send-introspection-query <url>
```
