[@bs.module] external gql: ReasonApolloTypes.gql = "graphql-tag";

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

let component = ReasonReact.statelessComponent("Query");

let make = _children => {
  ...component,
  render: _self =>
    <GetAllPersonsQuery>
      ...(
           ({result, subscribeToMore}) =>
             <div>
               <h1> ("Persons: " |> ste) </h1>
               (
                 switch (result) {
                 | Error(_e) =>
                   "Something Went Wrong" |> ste;
                 | Loading => "Loading" |> ste
                 | Data(response) => 
                    <ShowLivePersons 
                      persons={response##allPersons} 
                      getLiveData={
                        () => {
                          let _unsub = subscribeToMore(
                            ~document=newPersonAST,
                            ~updateQuery={(prev, next) => {
                              let addNewMessageJS = [%bs.raw {|
                                function(prev, next) {
                                  if(!next.subscriptionData.data || !next.subscriptionData.data.person)
                                    return prev;
                                  return Object.assign({}, prev, {
                                    messages: prev.allPersons.concat(next.subscriptionData.data.person)
                                  });
                                }
                              |}];
                              addNewMessageJS(prev, next);                              
                            }},
                            () 
                          );
                        }
                      }
                    />
                 }
               )
             </div>
         )
    </GetAllPersonsQuery>,
};
