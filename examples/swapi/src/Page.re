let ste = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("Page");

let make = _children => {
  ...component,
  render: _self =>
    <div>
      <h1> (ste("Star Wars")) </h1>
      <h2> (ste("Movies")) </h2>
      <Feed />
      <h2> (ste("Characters")) </h2>
      <Characters queryClient=(module Client.Instance.Query) />
    </div>
};
