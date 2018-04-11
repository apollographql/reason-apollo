let ste = ReasonReact.stringToElement;

module CharacterQueryModule = [%graphql
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

let parseFilms = allFilms => {
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
};

module CharacterQuery = ReasonApollo.CreateQuery(CharacterQueryModule);

let component = ReasonReact.statelessComponent("Query");

let make = _children => {
  ...component,
  render: _self => {
    let characterQuery = CharacterQueryModule.make(~first=6, ());
    <CharacterQuery variables=characterQuery##variables>
      ...(
        ({data}) =>
          <div>
            <h1> ("Star Wars 2: " |> ReasonReact.stringToElement) </h1>
            (
              switch (data) {
              | NoData => "No Data" |> ReasonReact.stringToElement
              | Error(_) =>
                "Something Went Wrong" |> ReasonReact.stringToElement
              | Loading => "Loading" |> ReasonReact.stringToElement
              | Data(result) =>
                switch (result##allFilms) {
                | None => "No Films" |> ReasonReact.stringToElement
                | Some(films) =>
                  films
                  |> parseFilms
                  |> Array.mapi((index, film) => <li key=(index |> string_of_int)> (ste(film)) </li>)
                  |> ReasonReact.arrayToElement                        
                }
              } 
            )
          </div>
      )
    </CharacterQuery>;
  }
};
