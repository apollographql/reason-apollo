module Styles = {
  open Css;
  
  let header = style([
    backgroundColor(Theme.primaryColor),
    color(white),
    fontSize(rem(2.0)),
    padding(px(20))
  ]);
};

let component = ReasonReact.statelessComponent("Header");

let make = _children => {
  ...component,
  render: _self => <div className=Styles.header> ("Chat Application" |> ReasonReact.string) </div>,
};
