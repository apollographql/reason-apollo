## Reason DOJO

### Real time chat application with Reason and GraphQL

This example shows Reason Apollo usage with Queries, Mutations and Subscriptions

### Config

- Your GraphQL endpoint: https://boiling-bastion-96890.herokuapp.com/graphql
- The documentation of the GraphQL endpoint (Note! We didn't wrote any documentation, just a normal GraphQL API):
https://graphqlbin.com/v2/WLPqS6 (We recommend using Chrome)
- Paper, pen, redbull,...


### TODO

#### GraphQL Playground

Open the GraphQL playground in your browser https://graphqlbin.com/v2/WLPqS6 (Safari doesn't work)
Add a user and grab it's id to make mutations

#### Run the app
`yarn`
`yarn start`
`yarn webpack`

#### Setup GraphQL PPX
Run the introspection query, so the ppx can typecheck based on the graphql_schema : 
https://github.com/apollographql/reason-apollo#send-introspection-query

