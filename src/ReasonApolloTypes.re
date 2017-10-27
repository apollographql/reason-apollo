/*type wrapper = (ReasonReact.reactClass => ReasonReact.reactClass) [@bs];*/
type queryString;
type gql = (string => queryString) [@bs];
/*type graphql = (queryString => wrapper) [@bs];*/

