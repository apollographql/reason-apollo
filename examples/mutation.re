
/*
module PokemonMutation = [%graphql {|
    mutation addPokemon($name: String!) {
       addPokemon(name: $name) {
            name
        }
    }
|}];
      
let component = ReasonReact.statelessComponent("Greeting");

module Mutation = Client.Instance.Mutation;

let make = (_children) => {
...component,
render: (_) => {    
    <Mutation>
    ...((mutate, response) => {
      let pokemonMutation = PokemonMutation.make(~name="Reason", ());        
      switch response {
         | NotCalled => {
            mutate(pokemonMutation);
            <div> (ReasonReact.stringToElement("Loading")) </div>
         }
         | Loading => <div> (ReasonReact.stringToElement("Loading")) </div>
         | Failed(error) => <div> (ReasonReact.stringToElement(error)) </div>
         | Loaded(result) => {
            let parse = pokemonMutation##parse;
            let pokemonName = parse(result)##addPokemon##name;
            <div> (ReasonReact.stringToElement(pokemonName)) </div>
         }
    }})
  </Mutation>
}
};
*/
