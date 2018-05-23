let ste = ReasonReact.string;

type action =
  | ChangeId(string);

type state = {id: string};

let handleClick = event =>
  if (! ReactEventRe.Mouse.defaultPrevented(event)) {
    ReactEventRe.Mouse.preventDefault(event);
  };

let reducer = (action, state) =>
  switch (action) {
  | ChangeId(id) => ReasonReact.Update({...state, id})
  };

let initialState = () => {id: ""};

let onIdInputChange = ({ReasonReact.send}, e) =>
  send(
    ChangeId(
      ReactDOMRe.domElementToObj(ReactEventRe.Form.target(e))##value,
    ),
  );

let component = ReasonReact.reducerComponent("PersonById");

let make = _children => {
  ...component,
  reducer,
  initialState,
  render: self =>
    <div>
      <form>
        <h1> ("Get Person By Id" |> ste) </h1>
        <input
          autoFocus=true
          placeholder="Get this id"
          _type="text"
          value=self.state.id
          onChange=(onIdInputChange(self))
          required=true
        />
        <GetPersonById id=self.state.id />
      </form>
    </div>,
};
/* let ste = ReasonReact.string;

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

   let component = ReasonReact.statelessComponent("Query");

   let make = _children => {
     ...component,
     render: _self =>
       <GetAllPersonsQuery>
         ...(
              ({result}) =>
                <div>
                  <h1> ("Persons: " |> ste) </h1>
                  (
                    switch (result) {
                    | Error(e) =>
                      Js.log(e);
                      "Something Went Wrong" |> ste;
                    | Loading => "Loading" |> ste
                    | Data(response) =>
                      response##allPersons
                      |> Array.mapi((index, person) =>
                           <div key=(index |> string_of_int)>
                             (person##name |> ste)
                             <br />
                             <p> ("ID: " ++ person##id |> ste) </p>
                           </div>
                         )
                      |> ReasonReact.array
                    }
                  )
                </div>
            )
       </GetAllPersonsQuery>,
   }; */