module Styles = {
  open Css;
  let container = userMsg => style([
    margin(px(5)),
    padding(px(5)),
    backgroundColor(Theme.secondaryColor),
    color(white),
    borderRadius(px(3)),
    width(px(300))
  ]);
};

let component = ReasonReact.statelessComponent("Message");

let make = (~message, _children) => {
  ...component,
  render: _ =>
    <div className={Styles.container(message##author##id === "my-id")}>
      (message##author##name ++ " : " ++ message##text |> ReasonReact.string)
    </div>,
};
