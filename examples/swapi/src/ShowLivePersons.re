let ste = ReasonReact.string;

let component = ReasonReact.statelessComponent("ShowLivePersons");

let make = (
  ~persons,
  ~getLiveData,
  _children
) => {
  ...component,
  didMount: _self => {
    getLiveData();
  },
  render: _self => 
    persons 
    |> Array.mapi((index, person) =>
      <div key=(index |> string_of_int)>
      (person##name |> ste)
      <br />
      <p> ("ID: " ++ person##id |> ste) </p>
      </div>
    )
    |> ReasonReact.array
};
