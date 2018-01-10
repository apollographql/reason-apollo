[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

module QueryConfig = {
    /* 
        WE DEFINED A QUERY, BUT IT DONT NECESSERALY MATCH OUR SCHEMA 
    */
    let query = [@bs] gql({|
        query getPokemon($name: String!){
            pokemon(name: $name) {
                name
            }
        }
    |});  
    
    /* Describe the result type */
    /* 
        WE DEFINED TYPES, BUT THEY DONT NECESSERALY MATCH OUR QUERY 
    */
    type pokemon = {. "name": string};
    type response = {. "pokemon": pokemon};
        
    /* Optional variables passed to the query */
    type variables = {. "name": string}; /* or `type variables;` if none */
};
      
let component = ReasonReact.statelessComponent("Greeting");

module FetchUserName = Create_apollo_client.Client.Query(QueryConfig);

let variables = {
  "name": "Pikachu"
};

let make = (_children) => {
...component,
render: (_) => 
  <FetchUserName variables>
    (response => {
      switch response {
         | Loading => <div> (ReasonReact.stringToElement("Loading")) </div>
         | Failed(error) => <div> (ReasonReact.stringToElement(error)) </div>
         /* 
            WE HAVE TYPE  SAFETY BASED ON TYPES DEFINED ABOVE. 
            BUT NOT SURE IT WAS CORRECT AGAINST SCHEMA
        */
         | Loaded(result) => <div> (ReasonReact.stringToElement(result##pokemon##name)) </div>
    }})
  </FetchUserName>
};