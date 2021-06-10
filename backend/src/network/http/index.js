'use strict';
var glob = require('glob'),
  path = require('path');

module.exports = async function (app) {
  glob.sync(__dirname + '/routes/**/*.js').forEach(function (file) {
    require(path.resolve(file))(app);
  });
};
