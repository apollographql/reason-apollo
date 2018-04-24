const path = require('path');
const isProd = process.env.NODE_ENV === "production";

module.exports = {
  entry: './src/index.bs.js',
  mode: isProd ? "production" : "development",
  output: {
    path: path.join(__dirname, "build"),
    filename: 'bundle.js',
  },
};
