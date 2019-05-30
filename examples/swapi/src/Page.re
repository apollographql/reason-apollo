[@react.component]
let make = () =>
  <div>
    <h1> {React.string("Star Wars")} </h1>
    <Persons />
    <AddPerson />
    <GetPerson />
    <PersonById />
    <PersonByIdDelete />
    <DeletePerson />
    <SubscribeToPersons />
  </div>;
