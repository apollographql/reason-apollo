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

[@react.component]
let make = (~id) => {
    let deletePersonMutation = DeletePerson.make(~id, ());
    <DeletePersonMutation>
      ...(
           (mutation, {result}) =>
             <div>
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
               <span>
                 (
                   switch (result) {
                   | NotCalled =>
                     Js.log("Not called");
                     "" |> ste;
                   | Data(d) =>
                     Js.log2("data", d);
                     let person = d##deletePerson;
                     Js.log2("deleted person", person);
                     "Person with this ID has been deleted" |> ste;
                   | Error(e) =>
                     Js.log2("error", e);
                     "ERROR" |> ste;
                   | Loading =>
                     Js.log("Loading");
                     "Loading" |> ste;
                   }
                 )
               </span>
             </div>
         )
    </DeletePersonMutation>;
};
