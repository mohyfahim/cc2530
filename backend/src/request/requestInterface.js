var multiparty = require("multiparty");

class ReqInterface {
  requestInput = async (params) => {
    // Controller.toLogApi("request", params, params.body);

    let requestBody;
    let tmpInput = await this.formInput(params);
    if (Object.keys(tmpInput.data).length > 0) {
      requestBody = tmpInput.data;
    } else if (Object.keys(tmpInput.formData).length > 0) {
      requestBody = this.normalizeForm(tmpInput.formData);
    } else if (Object.keys(tmpInput.files).length > 0) {
      requestBody = this.normalizeForm(tmpInput.files);
    } else {
      requestBody = {};
    }
    return requestBody;
  };

  formInput = async (params) => {
    var form = new multiparty.Form();
    let Data, formData;
    return new Promise(async (resolve, reject) => {
      form.parse(params, async function (err, fields, files) {
        Data = params.body !== undefined ? params.body : {};
        formData = fields !== undefined ? fields : {};
        files = files !== undefined ? files : {};
        resolve({ data: Data, formData: formData, files: files });
      });
    });
  };

  normalizeForm = (jsonData) => {
    let obj = {};
    Object.keys(jsonData).forEach(function (key) {
      obj[key] = jsonData[key][0];
    });
    return obj;
  };
}
module.exports = new ReqInterface();
