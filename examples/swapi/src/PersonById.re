let ste = React.string;

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
        onChange={event => {
          let id = ReactEvent.Form.target(event)##value;
          setId(_ => id);
        }}
        required=true
      />
      <GetPersonById id />
    </form>
  </div>;
};
