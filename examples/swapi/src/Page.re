let component = ReasonReact.statelessComponent("Page");

let make = _children => {
  ...component,
  render: _self =>
    <div> <h1> (ReasonReact.stringToElement("Star Wars")) </h1> <Feed /> </div>
};
