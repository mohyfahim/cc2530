require('dotenv').config();
const http = require('http');
const express = require('express');
const loaders = require('./src/loaders');
var Route = require('./src/network/http');

async function mainServer() {
  const app = express();
  const server = http.createServer(app);
  await loaders({ app });

  Route(app);
  server.listen(process.env.PORT, process.env.HOST, () => {
    console.log(
      `Server running at http://${process.env.HOST}:${process.env.PORT}/`,
    );
  });
}

try {
  mainServer();
} catch (error) {
  console.log({ status: error })
}
