let ste = ReasonReact.string;

module AddPerson = [%graphql {|
    mutation addPerson($age: Int!, $name: String!) {
        createPerson(age: $age, name: $name) {
          name
        }
      }
|}];

let component = ReasonReact.statelessComponent("AddPerson");

module AddPersonMutation = ReasonApollo.CreateMutation(AddPerson);

let make = (_children) => {
    ...component,
    render: _self => {
        let addPersonMutation = AddPerson.make(~name="Bob", ~age=24, ());

        <AddPersonMutation>
            ...(
                (mutation, {result})  => {
                  <div>
                    <button onClick=((_) => {
                        mutation(~variables=addPersonMutation##variables, ~refetchQueries=[|"getAllPersons"|], ()) |> ignore;
                        Js.log("SEND");
<<<<<<< HEAD
                    })> 
                      ("Add a person" |> ste) 
                    </button>
                    <span>
                    {
                      switch result {
                        | NotCalled => { Js.log("Not called"); "" |> ste }
                        | Data(d) => {Js.log2("data", d); ("Bob has been added") |> ste}
                        | Error(e) => { Js.log2("error", e); "ERROR" |> ste }
                        | Loading => {Js.log("Loading"); "Loading" |> ste }
                      }
                    }
                    </span>
                    </div>
=======
                    })> ("Add a person" |> ReasonReact.string) </button>
>>>>>>> 681326a2bd4ddc5c4ae9f039ed1aa18087037139
                }   
            )
        </AddPersonMutation>
    }
};
