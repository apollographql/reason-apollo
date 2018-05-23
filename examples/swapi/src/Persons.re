let ste = ReasonReact.string;

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
  render: _self =>
    <GetAllPersonsQuery>
      ...(
           ({result}) =>
             <div>
               <h1> ("Persons: " |> ste) </h1>
               (
                 switch (result) {
                 | Error(e) =>
                   Js.log(e);
                   "Something Went Wrong" |> ste;
                 | Loading => "Loading" |> ste
                 | Data(response) =>
                   response##allPersons
                   |> Array.mapi((index, person) =>
                        <div key=(index |> string_of_int)>
                          (person##name |> ste)
                          <br />
                          <p> ("ID: " ++ person##id |> ste) </p>
                        </div>
                      )
                   |> ReasonReact.array
                 }
               )
             </div>
         )
    </GetAllPersonsQuery>,
};