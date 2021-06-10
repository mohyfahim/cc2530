class Response {

    async response(result,code,message) {
        let resp = {};
        resp.hasError = false;
        if (code != 200){
            resp.hasError = true;
            resp.message = message;
        }else{
            resp.result = result;
        }
        resp.statusCode = code;
        return resp;

    }

    async dbResponse(result,code,message){
        let resp = {};
        resp.hasError = false;
        if (code != 1){
            resp.hasError = true;
            resp.message = message;
        }else{
            resp.result = result;
        }
        resp.code = code;

        return resp;
    }

}

module.exports = new Response();