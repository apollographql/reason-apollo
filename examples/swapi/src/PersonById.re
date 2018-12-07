let ste = ReasonReact.string;

type action =
  | ChangeId(string);

type state = {id: string};

let reducer = (action, state) =>
  switch (action) {
  | ChangeId(id) => ReasonReact.Update({...state, id})
  };

let initialState = () => {id: ""};

let onIdInputChange = ({ReasonReact.send}, e) =>
  send(
    ChangeId(
      ReactDOMRe.domElementToObj(ReactEventRe.Form.target(e))##value,
    ),
  );

let component = ReasonReact.reducerComponent("PersonById");

let make = _children => {
  ...component,
  reducer,
  initialState,
  render: self =>
    <div>
      <form>
        <h1> ("Get Person By Id" |> ste) </h1>
        <input
          autoFocus=true
          placeholder="Get this id"
          type_="text"
          value=self.state.id
          onChange=(onIdInputChange(self))
          required=true
        />
        <GetPersonById id=self.state.id />
      </form>
    </div>,
};