
module PokemonQuery = [%graphql {|
    query getPokemon($name: String!){
        pokemon(name: $name) {
            name
        }
    }
|}];
      
let component = ReasonReact.statelessComponent("Greeting");

module Query = Client.Instance.Query;

let make = (_children) => {
...component,
render: (_) => {
    let unexpectedError = <div> (ReasonReact.stringToElement("There was an internal error")) </div>;
    let pokemonQuery = PokemonQuery.make(~name="Pikachu", ());
    <Query query=pokemonQuery>
    ((response, parse) => {
      switch response {
         | Loading => <div> (ReasonReact.stringToElement("Loading")) </div>
         | Failed(error) => <div> (ReasonReact.stringToElement(error)) </div>
         | Loaded(result) => {
            let pokemon = parse(result)##pokemon;
            switch pokemon {
                | Some(pokemon) => {
                    switch pokemon##name {
                    | Some(name) => <div> (ReasonReact.stringToElement(name)) </div>
                    | None => unexpectedError
                    }
                }
                | None => unexpectedError
            };             
         }
    }})
  </Query>
}
};