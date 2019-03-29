type renderPropObjJS = {
  .
  "loading": bool,
  "called": bool,
  "data": Js.Nullable.t(Js.Json.t),
  "error": Js.Nullable.t(ReasonApolloTypes.apolloError),
  "networkStatus": Js.Nullable.t(int),
  "variables": Js.Null_undefined.t(Js.Json.t),
};
