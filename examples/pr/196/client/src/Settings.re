module GetSettings = [%graphql
  {|
  query getSettings {
    settings {
      id
      version
      wallets {
        id
        name
      }
    }
  }
|}
];

module GetSettingsQuery = ReasonApollo.CreateQuery(GetSettings);

[@react.component]
let make = () => {
  <GetSettingsQuery>
    ...{({result}) =>
      switch (result) {
      | Loading => <div> {ReasonReact.string("Loading")} </div>
      | Error(error) => <div> {ReasonReact.string(error##message)} </div>
      | Data(response) =>
        Js.log(response);
        <div> {ReasonReact.string("Success")} </div>;
      }
    }
  </GetSettingsQuery>;
};
