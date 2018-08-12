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


type graphqlError = {
  .
  "message": string,
  "locations": Js.Nullable.t(array(string)),
  "path": Js.Nullable.t(array(string)),
  "nodes": Js.Nullable.t(array(string)),
};

type executionResult = {
  .
  "errors": Js.Nullable.t(Js.Array.t(graphqlError)),
  "data": Js.Nullable.t(Js.Json.t),
};

/* TODO define all types */
type operation = {
  .
  "query": queryString
};

/* TODO define subscription */
type subscription;

type errorResponse = {
  . 
  "graphqlError": Js.Nullable.t(Js.Array.t(graphqlError)),
  "networkError": Js.Nullable.t(networkError),
  "response": Js.Nullable.t(executionResult),
  "operation": operation ,
  "forward": operation => subscription,
};

module type Config = {let query: string; type t; let parse: Js.Json.t => t;};

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

/* 
 apollo link ws
 */


[@bs.deriving abstract]
type webSocketLinkOptionsT = {
  [@bs.optional] reconnect: bool
};

[@bs.deriving abstract]
type webSocketLinkT = {
  uri: string,
  options: webSocketLinkOptionsT
};

type documentNodeT;

type splitTest = {
  .
  "query": documentNodeT
};
