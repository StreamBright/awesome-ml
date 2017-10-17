const path = require('path');
const webpack = require('webpack');

module.exports = {
  entry: {
    incdec: './lib/js/src/incdec/app.js',
    time: './lib/js/src/time/main.js'
  },
  output: {
    path: path.join(__dirname, "bundledOutputs"),
    filename: '[name].js',
  },
  plugins: [
    new webpack.optimize.UglifyJsPlugin({
     minimize: true,
     compress: true
    })
  ]
};
