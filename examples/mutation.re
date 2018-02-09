
let ste = ReasonReact.stringToElement;

module AddPerson = [%graphql {|
    mutation addPerson($age: Int!, $name: String!) {
        createPerson(age: $age, name: $name) {
          name
        }
      }
|}];
      
let component = ReasonReact.statelessComponent("Greeting");

module Mutation = Client.Instance.Mutation;

let make = (_children) => {
...component,
render: (_) => {    
    <Mutation>
    ...((mutate, response) => {
      let addPersonMutation = AddPerson.make(~name="Reason", ~age=22, ());        
      switch response {
         | NotCalled => {
            mutate(addPersonMutation);
            <div> (ReasonReact.stringToElement("Loading")) </div>
         }
         | Loading => <div> (ReasonReact.stringToElement("Loading")) </div>
         | Failed(error) => <div> (ReasonReact.stringToElement(error)) </div>
         | Loaded(result) => {
            let parse = addPersonMutation##parse;
            let person = parse(result)##createPerson;
            switch person {
            | Some(person) => <div> (person##name |> ste) </div>
            | None => 
            <div> 
                (
                "backend didn't returned the deleted person (you probably forgot to handle it JS)" 
                |> ste
                ) 
            </div>
            };
         }
    }})
  </Mutation>
}
};
