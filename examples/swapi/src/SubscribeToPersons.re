let component = ReasonReact.statelessComponent("SubscribeToPersons");

module Persons = [%graphql
{|
  subscription subscribeToPerons {
    person: Person {
      node {
        name
      }
    } 
  }
|}
];

module PersonsSubscription = ReasonApollo.CreateSubscription(Persons);

let make = (
  children  
) => {
  ...component,
  render: _self => 
  <PersonsSubscription>
    ...({result} => 
     switch result {
      | Error(e) => "Something went wrong" |> ste
      | Loading => "Loading" => ste
      | Data(response) => switch response##person {
          | Some(person) => person##name
          | None => "Person not found"
        }
     }
     )
  </PeronsSubscription>
};
