let ste = ReasonReact.stringToElement;

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
        let addPersonMutation = AddPerson.make(~age=42, ~name="John", ());
        <AddPersonMutation variables=addPersonMutation##variables>
            ...(
                (mutation, _)  => {
                    <button onClick=((_) => {
                        mutation(Js.Nullable.null) |> ignore;
                        Js.log("SEND");
                    })> ("Add a person" |> ReasonReact.stringToElement) </button>
                }   
            )
        </AddPersonMutation>
    }
};