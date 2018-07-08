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
           ({result, fetchMore}) =>
             <div>
               <h1> ("Persons: " |> ste) </h1>
               (
                 switch (result) {
                 | Error(e) =>
                   Js.log(e);
                   "Something Went Wrong" |> ste;
                 | Loading => "Loading" |> ste
                 | Data(response) =>
                   <div>
                     (
                       response##allPersons
                       |> Array.mapi((index, person) =>
                            <div key=(index |> string_of_int)>
                              (person##name |> ste)
                              <br />
                              <p> ("ID: " ++ person##id |> ste) </p>
                            </div>
                          )
                       |> ReasonReact.array
                     )
                     <button
                       onClick=(
                         _ =>
                           fetchMore(
                             ~updateQuery=
                               (prev, _next) => {
                                  /* Update Apollo Store with [@bs.raw {||}] for now, since the type comming in is a generic Js.Json.t for now*/
                                  prev;
                               },
                             (),
                           )
                           |> ignore
                       )>
                       ("fetchMore" |> ReasonReact.string)
                     </button>
                   </div>
                 }
               )
             </div>
         )
    </GetAllPersonsQuery>,
};
