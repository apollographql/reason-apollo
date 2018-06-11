let component = ReasonReact.statelessComponent("SubscribeToPersons");

module GetAllPersons = [%graphql
  {|
    subscription subscribeOnPersons {
      person: Person {
        node {
          id
          age
          name
        }
      }
    }
|}
];

module GetAllPersonsSubscription = ReasonApollo.CreateSubscription(GetAllPersons);

let make = (_children) => {
  ...component,
  render: (_) => <GetAllPersonsSubscription>
    ...{
      ({result}) => 
      switch result {
        | Loading => <div> {"Loading" |> ReasonReact.string} </div>
        | Error(_err) => <div> {"Error" |> ReasonReact.string} </div>
        | Data(result) => 
            switch result##person {
              | Some(person) =>
              
                switch person##node {
                  | Some(node) => <div> { node##name |> ReasonReact.string } </div>
                  | None => <div> {"No Person found" |> ReasonReact.string } </div>
                }
              | None => <div/>
        }
      }
    }
  </GetAllPersonsSubscription>
};
