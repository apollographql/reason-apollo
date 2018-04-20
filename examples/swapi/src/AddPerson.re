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
        let dic = Js.Dict.empty();
        Js.Dict.set(dic, "age", Js.Json.number(24 |> float_of_int));
        Js.Dict.set(dic, "name", Js.Json.string("Bob"));
        let variables = Js.Json.object_(dic);

        <AddPersonMutation>
            ...(
                (mutation, _)  => {
                    <button onClick=((_) => {
                        mutation({
                            "variables": Js.Nullable.return(variables),
                            "refetchQueries": [|"getAllPersons"|]
                        }) |> ignore;
                        Js.log("SEND");
                    })> ("Add a person" |> ReasonReact.stringToElement) </button>
                }   
            )
        </AddPersonMutation>
    }
};