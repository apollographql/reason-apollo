let ste = ReasonReact.stringToElement;

module GetAllPersons = [%graphql
  {|
    query getAllPersons {
      allPersons {
        id
        age
        name
      }
    }
|}
];

module GetAllPersonsQuery = ReasonApollo.CreateQuery(GetAllPersons);

let component = ReasonReact.statelessComponent("Query");

let make = _children => {
  ...component,
  render: _self => {
    let getAllPersonsQuery = GetAllPersons.make(());
    <GetAllPersonsQuery variables=getAllPersonsQuery##variables>
      ...(
        ({data}) =>
          <div>
            <h1> ("Persons: " |> ste) </h1>
            (
              switch (data) {
              | NoData => "No Data" |> ste
              | Error(_) =>
                "Something Went Wrong" |> ste
              | Loading => "Loading" |> ste
              | Data(response) =>
                  response##allPersons
                  |> Array.mapi((index, person) => <div key=(index |> string_of_int)> (person##name |> ste) </div>)
                  |> ReasonReact.arrayToElement                        
              }
            )
          </div>
      )
    </GetAllPersonsQuery>;
  }
};
