var bodyParser = require('body-parser');
var cors = require('cors');
const znp = require('cc-znp');

module.exports = async ({ app }) => {
  app.use(cors());
  app.use(bodyParser.json());
  app.use(bodyParser.urlencoded({ extended: true }));
  app.use(bodyParser.json());
  app.use(function (req, res, next) {
    res.header(
      'Access-Control-Allow-Origin',
      process.env.ALLOW_ORIGIN,
    ); // update to match the domain you will make the request from
    res.header(
      'Access-Control-Allow-Headers',
      process.env.ALLOW_HEADERS,
    );
    res.header('Access-Control-Allow-Methods', ['GET', 'PUT', 'POST'])
    next();
  });

  znp.init({
    path: '/dev/ttyUSB0',
    options: {
      baudRate: 115200,
      rtscts: false
    }
  }, function (err) {
    if (err)
      console.log(err);
  });
  znp.on('data', function (data) {
    console.log('data: ', data);  // The parsed data
  });

  znp.on('AREQ', function (msg) {
    console.log('AREQ: ', msg);
  });


  znp.on('ready', function () {
    console.log('Initialization completes.');
  });
};
