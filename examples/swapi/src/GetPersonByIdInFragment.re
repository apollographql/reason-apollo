/* Example of reading person id 123 from graphql cache using fragment */
let ste = React.string;

module GraphFragment = [%graphql
  {|
     fragment personFields on Person {
      id
      age
      name
     }
   |}
];

module PersonFragment = ApolloClient.ReadFragment(GraphFragment.PersonFields);

let getPersonByIdInFragment = personId =>
  PersonFragment.read(
    ~client=Client.instance,
    ~id="Person" ++ ":" ++ personId,
    ~fragmentName="personFields",
    (),
  );

[@react.component]
let make = (~id) => {
  let person = getPersonByIdInFragment(id);
  <div>
    {switch (person) {
     | Some(person) => <div> {person##name |> ste} </div>
     | None => <div> {ste("There's no person with that id")} </div>
     }}
  </div>;
};
