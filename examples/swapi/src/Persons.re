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
<<<<<<< HEAD
        ({result}) =>
          <div>
            <h1> ("Persons: " |> ste) </h1>
            (
              switch (result) {
              | Error(e) => {
                Js.log(e);
                "Something Went Wrong" |> ste                
              }
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
=======
           ({result}) =>
             <div>
               <h1> ("Persons: " |> ste) </h1>
               (
                 switch (result) {
                 | NoData => "No Data" |> ste
                 | Error(e) =>
                   Js.log(e);
                   "Something Went Wrong" |> ste;
                 | Loading => "Loading" |> ste
                 | Data(response) =>
                   response##allPersons
                   |> Array.mapi((index, person) =>
                        <div key=(index |> string_of_int)>
                          (person##name |> ste)
                        </div>
                      )
                   |> ReasonReact.array
                 }
               )
             </div>
         )
    </GetAllPersonsQuery>,
};
>>>>>>> 681326a2bd4ddc5c4ae9f039ed1aa18087037139
