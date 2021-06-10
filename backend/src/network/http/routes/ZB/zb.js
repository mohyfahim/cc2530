const reqinfo = require("../../../../request/requestInterface");
const ZB = require("../../../../controller/zb");
var response = require("../../../../reponse/response").response;

module.exports = function (app) {
  var _prefix = "/api/zb/";


  app.post(_prefix + "readConfiguration", async (req, res) => {
    let input = await reqinfo.requestInput(req);
    let resp = await ZB.readConfiguration(input);
    res.status(resp.code).send(await response(resp.result, resp.code, resp.message));
  });

}
