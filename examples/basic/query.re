let ste = ReasonReact.stringToElement;
module GetAllPersonsQuery = [%graphql {|
    query getAllPersons {
        allPersons {
          id
          age
          name
        }
      }
|}];
      
let component = ReasonReact.statelessComponent("Greeting");

module Query = Client.Instance.Query;

let make = (_children) => {
...component,
render: (_) => {
    let personsQuery = GetAllPersonsQuery.make(());
    <Query query=personsQuery>
    ...((response, parse) => {
      switch response {
         | Loading => <div> ("Loading" |> ste) </div>
         | Failed(error) => <div> (error |> ste) </div>
         | Loaded(result) => {
            parse(result)##allPersons
            |> Array.map(person => <div> (person##name |> ste) </div>)
            |> ReasonReact.arrayToElement             
         }
    }})
  </Query>
}
};
