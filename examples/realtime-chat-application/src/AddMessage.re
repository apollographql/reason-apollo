module Styles = {
  open Css;
  let container = style([
    
  ]);

  let input = style([
    width(px(300)),
    height(px(22)),
    marginLeft(px(32)),
  ]);

  let submitBtn = style([
    height(px(28)),
  ]);
};

module AddMessage = [%graphql {|
  mutation addMessage {
    addMessage(userId: "34450090-7d16-11e8-812a-b1747e68ae92", text: "HI") {
      id
    }
  }
|}];

module AddMessageMutation = ReasonApollo.CreateMutation(AddMessage);

type state = {
  text: string
};

type action = 
| Change(string);

let component = ReasonReact.reducerComponent("AddMessage");

let make = _children => {
  ...component,
  initialState: () => { text: "" },
  reducer: (action, _state) => switch action {
    | Change(str) => ReasonReact.Update({text: str})
  },
  render: ({state: {text}, send}) =>
    <AddMessageMutation>
      ...{
        (mutation, _result) => {
          <div>
          <input
            className=Styles.input
            value=text
            onChange={e => send(Change(Handler.onChange(e)))}
          />
          <button 
            className=Styles.submitBtn
            onClick={_e => mutation() |> ignore}
          >
            ("SEND" |> ReasonReact.string)
          </button>
          </div>
        }
      }
    </AddMessageMutation>,
};
