let ste = React.string;

[@react.component]
let make = (~persons, ~getLiveData) => {
  React.useEffect0(() => {
    getLiveData();
    /* Nothing to clean */
    None;
  });
  persons
  |> Array.mapi((index, person) =>
       <div key={index |> string_of_int}>
         {person##name |> ste}
         <br />
         <p> {"ID: " ++ person##id |> ste} </p>
       </div>
     )
  |> React.array;
};
