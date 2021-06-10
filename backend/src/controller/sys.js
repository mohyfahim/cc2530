
const logger = require("../log/log");
const SYS = require('cc-znp');

class sysController {
    constructor() {
        // this.init();
    }
    async init() {
        // await this.client.connectToMongo(process.env.DB_NAME);

    };
    async osalNvRead(json) {
        if (!json.id) {
            return { status: false, message: "id", code: 400 }
        }
        try {

            let result = new Promise((resolve, reject) => {
                SYS.sysRequest("osalNvRead", json, function (err, result) {
                    resolve(result);
                });
            }).then(value => { return value; });

            return { status: true, result: await result, code: 200 }


        } catch (e) {
            return { status: false, message: e.toString(), code: 400 }

        }

    }
    async osalNvWrite(json) {
        if (!json.id) {
            return { status: false, message: "id", code: 400 }
        }
        try {

            let result = new Promise((resolve, reject) => {
                SYS.sysRequest("osalNvWrite", json, function (err, result) {
                    resolve(result);
                });
            }).then(value => { return value; });

            return { status: true, result: await result, code: 200 }


        } catch (e) {
            return { status: false, message: e.toString(), code: 400 }

        }

    }
}
module.exports = new sysController();
