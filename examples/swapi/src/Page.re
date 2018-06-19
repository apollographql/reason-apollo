let component = ReasonReact.statelessComponent("Page");

let make = _children => {
  ...component,
  render: _self =>
    <div>
      <h1> (ReasonReact.string("Star Wars")) </h1>
      <Persons />
      <AddPerson />
      <GetPerson />
      <PersonById />
      <PersonByIdDelete />
      <DeletePerson />
      <SubscribeToPersons/>
    </div>,
};
