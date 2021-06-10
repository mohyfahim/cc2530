const reqinfo = require("../../../../request/requestInterface");
const SYS = require("../../../../controller/sys");
var response = require("../../../../reponse/response").response;

module.exports = function (app) {
  var _prefix = "/api/sys/";


  app.post(_prefix + "osalNvRead", async (req, res) => {
    let input = await reqinfo.requestInput(req);
    let resp = await SYS.osalNvRead(input);
    res.status(resp.code).send(await response(resp.result, resp.code, resp.message));
  });

  app.post(_prefix + "osalNvWrite", async (req, res) => {
    let input = await reqinfo.requestInput(req);
    let resp = await SYS.osalNvWrite(input);
    res.status(resp.code).send(await response(resp.result, resp.code, resp.message));
  });

}
