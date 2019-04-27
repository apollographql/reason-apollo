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

[@react.component]
let make = () => {
  <div>
    <h2> {"Person Subscription" |> React.string} </h2>
    <PersonsSubscription>
      ...{({result}) =>
        switch (result) {
        | Error(_e) =>
          Js.log(_e);
          "Something went wrong" |> React.string;
        | Loading => "Loading" |> React.string
        | Data(response) =>
          switch (response##person) {
          | Some(person) =>
            switch (person##node) {
            | Some(node) => node##name |> React.string
            | None => "No node found" |> React.string
            }
          | None => "Persons not found" |> React.string
          }
        }
      }
    </PersonsSubscription>
  </div>;
};
