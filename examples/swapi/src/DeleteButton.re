let ste = ReasonReact.string;

module DeletePerson = [%graphql
  {|
    mutation deletePerson($id: ID!) {
        deletePerson(id: $id) {
          id
        }
      }
|}
];

module DeletePersonMutation = ReasonApollo.CreateMutation(DeletePerson);

let component = ReasonReact.statelessComponent("DeleteButton");

let make = (~id, _children) => {
  ...component,
  render: _self => {
    let deletePersonMutation = DeletePerson.make(~id, ());
    <DeletePersonMutation>
      ...(
           (mutation, _) =>
             <button
               onClick=(
                 (_) =>
                   mutation(
                     ~variables=deletePersonMutation##variables,
                     ~refetchQueries=[|"getAllPersons"|],
                     (),
                   )
                   |> ignore
               )>
               ("Delete ID" |> ste)
             </button>
         )
    </DeletePersonMutation>;
  },
};