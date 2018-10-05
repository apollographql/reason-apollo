## Reason DOJO

### Real time chat application with Reason and GraphQL

This example shows Reason Apollo usage with Queries, Mutations and Subscriptions

### Config

- Your GraphQL endpoint: https://boiling-bastion-96890.herokuapp.com/graphql
- The documentation of the GraphQL endpoint (Note! We didn't wrote any documentation, just a normal GraphQL API):
https://graphqlbin.com/v2/WLPqS6 (We recommend using Chrome)
- Paper, pen, redbull,...


### SETUP

#### GraphQL Playground
Open the GraphQL playground in your browser https://graphqlbin.com/v2/WLPqS6 (Safari doesn't work)
Add a user, grab it's id, put the id into AddMessage.re to make mutations.
(Don't assume endpoint has existing valid user id hardcoded in src)
`
mutation { 
  addUser(name:"MyUser") {
    id
    name
  }
}
`

#### Run the app
`yarn`
`yarn start`
`yarn webpack`

(note: the send text field seems to be unused and hard coded to send "HI")

#### Setup GraphQL PPX
Run the introspection query, so the ppx can typecheck based on the graphql_schema : 
https://github.com/apollographql/reason-apollo#send-introspection-query


