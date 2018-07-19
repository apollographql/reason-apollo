module NewNotification = [%graphql {|
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

module NewNotificationSub = ReasonApollo.CreateSubscription(NewNotification);


let component = ReasonReact.statelessComponent("NewMessageNotification");

let make = _children => {
  ...component,
  render: _self => 
    <NewNotificationSub>
      ...{
        ({result}) => {
          switch result {
            | Loading => <div/>
            | Error(_error) => <div/>
            | Data(_response) =>
             <audio autoPlay=true>
              <source src="notification.ogg" _type="audio/ogg" />
              <source src="notification.mp3" _type="audio/mpeg" />
            </audio>
          }
        }
      }
    </NewNotificationSub>
};
