/**
 * An abstract type to describe a query string object.
 */
type queryString;

/**
 * The signature of the `graphql-tag/gql` function that transforms a GraphQL
 * query string to the standard GraphQL AST.
 * https://github.com/apollographql/graphql-tag
 */
type gql = [@bs] (string => queryString);

/**
 * An abstract type to describe an Apollo Link object.
 */
type apolloLink;

/**
 * An abstract type to describe an Apollo Cache object.
 */
type apolloCache;

type networkError = {. "statusCode": int};

/* TODO: define missing keys */
type apolloLinkErrorResponse = {. "networkError": Js.Nullable.t(networkError)};

module type Config = {let query: string; type t; let parse: Js.Json.t => t;};

type graphqlError = {
  .
  "message": string,
  "locations": Js.Nullable.t(array(string)),
  "path": Js.Nullable.t(array(string)),
  "nodes": Js.Nullable.t(array(string)),
};

type apolloError = {
  .
  "message": string,
  "graphQLErrors": Js.Nullable.t(array(string)),
  "networkError": Js.Nullable.t(string)
};

type apolloOptions = {
    .
    "query": queryString,
    "variables": Js.Json.t,
};
