module Styles = {
  open Css;
  
  let container = style([
    marginBottom(px(100))
  ]);
};

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <div className=Styles.container>
      <Header/>
      <MessagesContainer />
      <AddMessage />
      <NewMessageNotification />
    </div>,
};
