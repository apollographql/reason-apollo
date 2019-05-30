[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

module Styles = {
  open Css;
  let container = style([
    margin(px(20)),
    padding(px(5)),
    boxShadow(~x=px(2), ~y=px(2), grey) 
  ]);
};

module GetAllMessages = [%graphql {|
  {
    messages {
      id
      text
      author {
        id
        name
      }
    }
  }
|}];

module GetAllMessagesQuery = ReasonApollo.CreateQuery(GetAllMessages);


module MessageAdded = [%graphql {|
subscription messageAdded {
  messageAdded {
    id
    text
    author {
      id
      name
    }
  }
}
|}];

let messageAdded = MessageAdded.make();
let messageAddedASTQuery = gql(. messageAdded##query);

let component = ReasonReact.statelessComponent("Messages");

let make = _children => {
  ...component,
  render: _self => <div className=Styles.container>
    <GetAllMessagesQuery>
      ...{
        ({result, subscribeToMore}) => switch result {
            | Loading => <div> {"Loading" |> ReasonReact.string} </div>
            | Error(_e) => <div> {"Error" |> ReasonReact.string} </div>
            | Data(response) => 
              <Messages 
                onLoad={
                  () => {
                  let _unsub = subscribeToMore(
                    ~document=messageAddedASTQuery,
                    ~updateQuery={(prev, next) => {
                      let addNewMessageJS = [%bs.raw {|
                          function(prev, next) {
                            if(!next.subscriptionData.data || !next.subscriptionData.data.messageAdded)
                              return prev;
                            return Object.assign({}, prev, {
                              messages: prev.messages.concat(next.subscriptionData.data.messageAdded)
                            });
                          }
                      |}];
                      addNewMessageJS(prev, next);
                    }},
                    ()
                  )
                }
                } 
                messages=(response##messages) 
              />
          } 
      }
    </GetAllMessagesQuery>
</div>
};