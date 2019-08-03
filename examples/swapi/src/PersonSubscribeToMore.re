[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

let ste = React.string;

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

module NewPerson = [%graphql
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

let newPerson = NewPerson.make();
let newPersonAST = gql(. newPerson##query);

let updateQuerySubscription: ReasonApolloQuery.updateQuerySubscriptionT = [%bs.raw
  {|
      function(prev, next) {
        if(!next.subscriptionData.data || !next.subscriptionData.data.person)
          return prev;
        return Object.assign({}, prev, {
          messages: prev.allPersons.concat(next.subscriptionData.data.person)
        });
      }
  |}
];

[@react.component]
let make = () => {
  <GetAllPersonsQuery>
    ...{({result, subscribeToMore}) =>
      <div>
        <h1> {"Persons: " |> ste} </h1>
        {switch (result) {
         | Error(_e) => "Something Went Wrong" |> ste
         | Loading => "Loading" |> ste
         | Data(response) =>
           <ShowLivePersons
             persons={response##allPersons}
             getLiveData={() =>
               subscribeToMore(
                 ~document=newPersonAST,
                 ~updateQuery=updateQuerySubscription,
                 (),
               )
             }
           />
         }}
      </div>
    }
  </GetAllPersonsQuery>;
};
