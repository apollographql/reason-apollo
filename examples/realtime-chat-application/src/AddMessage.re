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
    addMessage(userId: "e4315d20-fa78-11e8-877a-8f1cf822dad6", text: "HI") {
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
