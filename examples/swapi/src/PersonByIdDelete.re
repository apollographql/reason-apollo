let ste = React.string;

let onIdInputChange = (setId, e) => setId(ReactEvent.Form.target(e)##value);

[@react.component]
let make = () => {
  let (id, setId) = React.useState(() => "");
  <div>
    <form>
      <h1> {"Delete Selected Person By Id" |> ste} </h1>
      <p> {"Select an ID from above and paste it into box" |> ste} </p>
      <input
        autoFocus=true
        placeholder="Delete this id"
        type_="text"
        value=id
        onChange={onIdInputChange(setId)}
        required=true
      />
    </form>
    <DeletePersonButton id />
  </div>;
};
