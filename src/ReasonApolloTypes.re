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
type apolloLinkErrorResponse = {. "networkError": option(networkError)};

module type Config = {let query: string; type t; let parse: Js.Json.t => t;};

type apolloError;

type apolloOptions = {
    .
    "query": queryString,
    "variables": Js.Json.t,
};