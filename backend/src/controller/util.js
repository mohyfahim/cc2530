
const logger = require("../log/log");
const UTIL = require('cc-znp');

class utilController {
    constructor() {
        // this.init();
    }
    async init() {
        // await this.client.connectToMongo(process.env.DB_NAME);

    };
    async getDeviceInfo(json) {
        
        try {

            let result = new Promise((resolve, reject) => {
                UTIL.utilRequest("getDeviceInfo", {}, function (err, result) {
                    resolve(result);
                });
            }).then(value => { return value; });

            return { status: true, result: await result, code: 200 }


        } catch (e) {
            return { status: false, message: e.toString(), code: 400 }

        }

    }

}
module.exports = new utilController();
