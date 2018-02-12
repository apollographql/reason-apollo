let ste = ReasonReact.stringToElement;

module CharacterQuery = [%graphql
  {|
    query films($first: Int) {
      allFilms(first: $first) {
        films {
          id
          title
          releaseDate
        }
      }
    }
|}
];

module Query = Client.Instance.Query;

let parseFilms = allFilms =>
  switch allFilms {
  | Some(allFilms) =>
    let films = allFilms##films;
    switch films {
    | Some(films) =>
      let parsedFilms =
        Js.Array.map(
          film =>
            switch film {
            | Some(film) =>
              let title = film##title;
              let releaseDate = film##releaseDate;
              switch (title, releaseDate) {
              | (Some(title), Some(releaseDate)) =>
                releaseDate ++ " - " ++ title
              | (Some(title), None) => title
              | (None, Some(releaseDate)) => releaseDate
              | (None, None) => "Error retrieving film"
              };
            | None => "Error retrieving film"
            },
          films
        );
      parsedFilms;
    | None => [||]
    };
  | None => [||]
  };

let component = ReasonReact.statelessComponent("Query");

let make = _children => {
  ...component,
  render: _self => {
    let characterQuery = CharacterQuery.make(~first=6, ());
    <Query query=characterQuery>
      ...(
           (response, parse) =>
             switch response {
             | Loading => <div> (ste("Loading")) </div>
             | Failed(error) => <div> (ste(error)) </div>
             | Loaded(result) =>
               let allFilms = parse(result)##allFilms;
               let parsedFilms = parseFilms(allFilms);
               <ul>
                 (
                   ReasonReact.arrayToElement(
                     Array.map(film => <li> (ste(film)) </li>, parsedFilms)
                   )
                 )
               </ul>;
             }
         )
    </Query>;
  }
};
