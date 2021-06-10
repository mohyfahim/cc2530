const reqinfo = require("../../../../request/requestInterface");
const UTIL = require("../../../../controller/util");
var response = require("../../../../reponse/response").response;

module.exports = function (app) {
  var _prefix = "/api/util/";


  app.post(_prefix + "getDeviceInfo", async (req, res) => {
    let input = await reqinfo.requestInput(req);
    let resp = await UTIL.getDeviceInfo(input);
    res.status(resp.code).send(await response(resp.result, resp.code, resp.message));
  });

  app.post(_prefix + "osalNvWrite", async (req, res) => {
    let input = await reqinfo.requestInput(req);
    let resp = await SYS.osalNvWrite(input);
    res.status(resp.code).send(await response(resp.result, resp.code, resp.message));
  });

}
