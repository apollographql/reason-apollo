let ste = ReasonReact.stringToElement;

module CharacterQuery = [%graphql
  {|
  query characters($first: Int) {
    allPeople(first: $first) {
      people {
        name
      }
    }
  }
|}
];

let parseCharacters =
  fun
  | Some(allCharacters) =>
    switch allCharacters##people {
    | Some(characters) =>
      Js.Array.map(
        character =>
          switch character {
          | Some(c) => switch (c##name) {
            | Some(name) => name
            | None => ""
            }
          | None => ""
          },
        characters
      )
    | None => [||]
    }
  | None => [||];

let component = ReasonReact.statelessComponent("Characters");

let make = (~queryClient, _children) => {
  ...component,
  render: _self => {
    let characterQuery = CharacterQuery.make(~first=10, ());
    module Query = (val (queryClient: (module ReasonApolloQuery.Query)));
    <Query query=characterQuery>
      ...(
           (response, parse) =>
             switch response {
             | Loading => <div> (ste("Loading")) </div>
             | Failed(error) => <div> (ste(error)) </div>
             | Loaded(result) =>
               let allCharacters = parse(result)##allPeople;
               let parsedFilms = parseCharacters(allCharacters);
               <ul>
                 (ReasonReact.arrayToElement(Array.map(film => <li> (ste(film)) </li>, parsedFilms)))
               </ul>;
             }
         )
    </Query>;
  }
};
