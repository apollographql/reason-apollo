module GetWallets = [%graphql
  {|
  query getWallets {
    wallets {
      id
      name
    }
  }
  |}
];

module GetWalletsQuery = ReasonApollo.CreateQuery(GetWallets);

[@react.component]
let make = () => {
  <GetWalletsQuery>
    ...{({result}) =>
      switch (result) {
      | Loading => <div> {ReasonReact.string("Loading")} </div>
      | Error(error) => <div> {ReasonReact.string(error##message)} </div>
      | Data(response) =>
        Js.log2("Wallets", response);
        <div> {ReasonReact.string("Success")} </div>;
      }
    }
  </GetWalletsQuery>;
};
