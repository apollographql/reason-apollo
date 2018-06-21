let ste = ReasonReact.string;

module AddPerson = [%graphql
  {|

    mutation addPerson($age: Int!, $name: String!) {
        createPerson(age: $age, name: $name) {
          name
        }
      }
|}
];

let component = ReasonReact.statelessComponent("AddPerson");

module AddPersonMutation = ReasonApollo.CreateMutation(AddPerson);

let make = _children => {
  ...component,
  render: _self => {
    let addPersonMutation = AddPerson.make(~name="Bob", ~age=24, ());
    <AddPersonMutation>
      ...(
           (mutation, {result}) =>
             <div>
               <button
                 onClick=(
                   (_) => {
                     mutation(
                       ~variables=addPersonMutation##variables,
                       ~refetchQueries=[|"getAllPersons"|],
                       (),
                     )
                     |> ignore;
                   }
                 )>
                 ("Add a person" |> ste)
               </button>
               <span>
                 (
                   switch (result) {
                   | NotCalled =>
                     "" |> ste;
                   | Data(d) =>
                     "Person has been added" |> ste;
                   | Error(e) =>
                     "ERROR" |> ste;
                   | Loading =>
                     "Loading" |> ste;
                   }
                 )
               </span>
             </div>
         )
    </AddPersonMutation>;
  },
};
