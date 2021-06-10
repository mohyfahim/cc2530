
const logger = require("../log/log");
const ZB = require('cc-znp');

class zbController {
    constructor() {
        // this.init();
    }
    async init() {
        // await this.client.connectToMongo(process.env.DB_NAME);

    };
    async readConfiguration(json) {
        if (!json.configid) {
            return { status: false, message: "config id", code: 400 }
        }
        try {

            let result = new Promise((resolve, reject) => {
                ZB.sapiRequest("readConfiguration", json, function (err, result) {
                    resolve(result);
                });
            }).then(value => { return value; });

            return { status: true, result: await result, code: 200 }


        } catch (e) {
            return { status: false, message: e.toString(), code: 400 }

        }

    }
}
module.exports = new zbController();
