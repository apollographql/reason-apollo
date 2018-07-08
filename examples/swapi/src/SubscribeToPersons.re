let component = ReasonReact.statelessComponent("SubscribeToPersons");

module Persons = [%graphql
{|

  subscription {
    person: Person {
      node {
        name
      }
    } 
  }
|}
];

module PersonsSubscription = ReasonApollo.CreateSubscription(Persons);

let make = _children => {
  ...component,
  render: _self => 
  <div>
    <h2> {"Person Subscription" |> ReasonReact.string} </h2>
   <PersonsSubscription>
    ...(({result}) => {
     switch result {
      | Error(_e) => { Js.log(_e); "Something went wrong" |> ReasonReact.string}
      | Loading => "Loading" |> ReasonReact.string
      | Data(response) => switch response##person {
          | Some(person) => 
              switch person##node {
                | Some(node) => node##name |> ReasonReact.string
                | None => "No node found" |> ReasonReact.string
              }
          | None => "Persons not found" |> ReasonReact.string
        }
     }
}
     )
  </PersonsSubscription>
  </div>
};
