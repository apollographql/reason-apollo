let ste = React.string;

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
let make = () => {
  /* pick a valid id from list returned from GetPersons query here
     "https://api.graph.cool/simple/v1/cjdgba1jw4ggk0185ig4bhpsn" and pass it to ~id variable
     */
  let deletePersonMutation =
    DeletePerson.make(~id="cjhhv0i51k5lf0160xszwdaps", ());
  <DeletePersonMutation>
    ...{(mutation, {result}) =>
      <div>
        <h1> {"Delete a Person By Id" |> ste} </h1>
        <p>
          {"Pick and Id from above and put it in DeletePerson.re" |> ste}
        </p>
        <button
          onClick={_ => {
            mutation(
              ~variables=deletePersonMutation##variables,
              ~refetchQueries=[|"getAllPersons"|],
              (),
            )
            |> ignore;
            Js.log("SEND");
          }}>
          {"Delete a person" |> ste}
        </button>
        <span>
          {switch (result) {
           | NotCalled =>
             Js.log("Not called");
             "" |> ste;
           | Data(d) =>
             Js.log2("data", d);
             "Person has been deleted" |> ste;
           | Error(e) =>
             Js.log2("error", e);
             "ERROR" |> ste;
           | Loading =>
             Js.log("Loading");
             "Loading" |> ste;
           }}
        </span>
      </div>
    }
  </DeletePersonMutation>;
};
