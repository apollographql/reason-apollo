const { ApolloServer, gql } = require('apollo-server');

// This is a (sample) collection of books we'll be able to query
// the GraphQL server for.  A more complete example might fetch
// from an existing data source like a REST API or database.

const wallets = [{
  id: "1",
  name: "What"
}, {
  id: "2",
  name: "Ever"
}]

const settings = {
  id: "1",
  version: "v1",
  wallets: wallets
}

// Type definitions define the "shape" of your data and specify
// which ways the data can be fetched from the GraphQL server.
const typeDefs = gql`
  type Wallet {
    id: ID!
    name: String!
  }

  type Settings {
    id: ID!
    version: String!
    wallets: [Wallet!]!
  }

  type Query {
    wallets: [Wallet!]!
    settings: Settings!
  }
`;

// Resolvers define the technique for fetching the types in the
// schema.  We'll retrieve books from the "books" array above.
const resolvers = {
  Query: {
    wallets: () => wallets,
    settings: () => settings
  },
};

// In the most basic sense, the ApolloServer can be started
// by passing type definitions (typeDefs) and the resolvers
// responsible for fetching the data for those types.
const server = new ApolloServer({ typeDefs, resolvers });

// This `listen` method launches a web-server.  Existing apps
// can utilize middleware options, which we'll discuss later.
server.listen().then(({ url }) => {
  console.log(`🚀  Server ready at ${url}`);
});
