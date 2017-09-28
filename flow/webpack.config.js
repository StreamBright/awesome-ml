const path = require('path');

module.exports = {
  entry: {
    async: './lib/js/src/async/app.js',
    flow: './lib/js/src/flow/flow.js',
    sierpinski: './lib/js/src/sierpinski/sierpinski.js',
  },
  output: {
    path: path.join(__dirname, "bundledOutputs"),
    filename: '[name].js',
  },
};
