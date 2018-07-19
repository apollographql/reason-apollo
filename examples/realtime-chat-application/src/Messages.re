let component = ReasonReact.statelessComponent("Messages");

let make = (
  ~messages,
 ~onLoad,
  _children
) => {
  ...component,
  didMount: _self => {
    onLoad()
  },
  render: _self => <div>
    {
      messages
      |> Js.Array.map(message => <ChatBubble  message key=message##id/>)
      |> ReasonReact.array
    }
  </div>
};
