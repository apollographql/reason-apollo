/* Example of reading person id 123 from graphql cache using fragment */
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

let personId = "123";

let res =
  PersonFragment.read(
    ~client=Client.instance,
    ~id="Person" ++ ":" ++ personId,
    ~fragmentName="personFields",
    (),
  );