let ste = ReasonReact.string;

/* alias Person as person because compiler doesn't like uppercase key names */
module GetPerson = [%graphql
  {|
    query getPerson ($id:ID!){
      person:
          Person(id:$id) {
            id
            age
            name
          }
        }
   |}
];

module GetPersonQuery = ReasonApollo.CreateQuery(GetPerson);

let component = ReasonReact.statelessComponent("Query");

let make = _children => {
  ...component,
  render: _self => {
    /* pick a valid id from list returned from GetPersons query here
       "https://api.graph.cool/simple/v1/cjdgba1jw4ggk0185ig4bhpsn" and pass it to ~id variable
       */
    let getPersonQuery = GetPerson.make(~id="cjdgbhnli36a10157gwxm9h97", ());
    <GetPersonQuery variables=getPersonQuery##variables>
      ...(
           ({result}) =>
             <div>
               <h1> ("Get Person: " |> ste) </h1>
               (
                 switch (result) {
                 | Error(e) =>
                   Js.log(e);
                   "Something Went Wrong" |> ste;
                 | Loading => "Loading" |> ste
                 | Data(response) =>
                   switch (response##person) {
                   | None => "No Person Data" |> ste
                   | Some(person) => <div> (person##name |> ste) </div>
                   }
                 }
               )
             </div>
         )
    </GetPersonQuery>;
  },
};