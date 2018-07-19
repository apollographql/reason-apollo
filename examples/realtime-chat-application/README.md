## Reason DOJO

### Real time chat application with Reason and GraphQL

### Config

- Your GraphQL endpoint: https://boiling-bastion-96890.herokuapp.com/graphql
- The documentation of the GraphQL endpoint (Note! We didn't wrote any documentation, just a normal GraphQL API):
https://graphqlbin.com/v2/WLPqS6 (We recommend using Chrome)
- Paper, pen, redbull,...


### TODO

#### GraphQL Playground

Open the GraphQL playground in your browser https://graphqlbin.com/v2/WLPqS6 (Safari doesn't work)
and follow the instructions in the comments

#### Run the app
`yarn`
`yarn start`
 You'll have a bunch of error messages, fix them in the next step.

#### Setup GraphQL PPX
Run the introspection query, so the ppx can typecheck based on the graphql_schema : 
https://github.com/apollographql/reason-apollo#send-introspection-query

#### ReasonReact in easy!
Add a header in the `App` 

#### Setup apolloClient
in `ReasonApolloClient`, setup a basic apollo client
https://github.com/apollographql/reason-apollo#create-the-apollo-client

In `Index`, enhance your application with the [ApolloProvider](https://github.com/apollographql/reason-apollo#apolloprovider)

#### Your first GraphQL query in Reason
in `MessagesContainer`, write your first GraphQL query with ReasonApollo. Copy paste the query you wrote in the playground.
 Once you have the messages, pass them to the `Messages` component. (It's important we keep this structure for the end of the app)

#### Displaying all the messages
in `Messages`, map over all messages and pass each message to the `ChatBubble` component

### SWEET!
You can view all messages now!
Let's add a message

#### ReasonReact state management is AWESOME
in `AddMessage`, transform the `statelessComponent` component in a [reducerComponent](https://reasonml.github.io/reason-react/docs/en/state-actions-reducer.html) and keep track of the input value in your state.
Make use of the `Handler` module, to help you get the value from the onChange event

#### Your first GraphQL Mutation in Reason
Add a GraphQL mutation with ReasonApollo (copy paste your mutation)
Use the hardcoded userId for now!


#### ApolloClient Setup ADVANCED
We need to 
- add a websocket link
- make use of the split function. This is its signature:
`type test = operation => bool;`
`type split = test => link1 => link2 => link`
Split, takes a test predicate, a link1 and another link2 and returns a link.

The purpose is to use the http link for queries and mutations and the websocket link for subscriptions.

There's an example doing just that: 
https://github.com/apollographql/reason-apollo/blob/master/examples/swapi/src/Client.re

### Choose your path
If you're running out of time, skip this and write the Notification.
(Note, you won't have live data if skipping, but an understanding of subscriptions)

#### LIVE data
in `MessagesContainer`, write your subscription.
IMPORTANT! don't use the Subscription component!

use `subscribeToMore` which is a field in the object passed down in the render prop of the `Query` component:
`({result, subscribeToMore}) => ...`

You only want to `subscribeToMore` when the `Messages` component mounted.
Here is the documentation about [subscribeToMore](https://www.apollographql.com/docs/react/advanced/subscriptions.html#subscribe-to-more) in react-apollo.

In reason-apollo,
```reason
[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";
  
let messageAdded = MessageAddedSubscription.make();
let subscriptionAST = gql(. messageAdded##query);

let _unsubscribe = subscribeToMore(
    ~document=subscriptionAST, 
    ~updateQuery={(prev, next) => { 
      /* HINT: this part will be fully auto typed in a near feature, it isn't for now. When you are in those circumstances, write JS in Reason ;) */
      let addNewMessage = [%bs.raw {|
          function(prev, next) {
            console.log('prev', prev);
            console.log('next', next);
            // TODO concatenate new message
            // ES6 is not configured, use Object.assign({}, prev, {})
            return prev;
          }
      |}];
      addNewMessage(prev, next);
    }},
    ()
  );
```

#### Notification
Write the subscription (copy paste the one you used in the playground)
Use the Subscription component to be notified of a subscription.
Everytime you have data, render a HTML5 `àudio` with `àutoPlay`. You have a notification.ogg and notification.mp3 available
```
<audio autoPlay=true>
  <source src="horse.ogg" type="audio/ogg">
  <source src="horse.mp3" type="audio/mpeg">
</audio>
```


## Congrats!


#### Bonus 
Improve some styling, store your userId in state



