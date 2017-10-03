const path = require('path');
const webpack = require('webpack');

module.exports = {
 // webpack: {
 //   plugins: [
 //     new webpack.DefinePlugin({
 //       'process.env': {
 //         NODE_ENV: JSON.stringify('production')
 //       }
 //     })
 //   ]
 // },
  entry: {
    async: './lib/js/src/async/app.js',
    flow: './lib/js/src/flow/flow.js'
  },
  output: {
    path: path.join(__dirname, "bundledOutputs"),
    filename: '[name].js',
  },
};
