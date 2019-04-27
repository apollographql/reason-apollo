let ste = React.string;

let onIdInputChange = (setId, e) =>
  setId(ReactDOMRe.domElementToObj(ReactEventRe.Form.target(e))##value);

[@react.component]
let make = () => {
  let (id, setId) = React.useState(() => "");
  <div>
    <form>
      <h1> {"Get Person By Id" |> ste} </h1>
      <input
        autoFocus=true
        placeholder="Get this id"
        type_="text"
        value=id
        onChange={onIdInputChange(setId)}
        required=true
      />
      <GetPersonById id />
    </form>
  </div>;
};
