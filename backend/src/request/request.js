var request = require('request')
const delay = require('delay');

class Request {
  async loopRequest(loop, request, para) {

    let response;
    for (let index = 0; index < loop; index++) {
      response = await this.RequestMaker(request, para)
      if (response.statusCode == 200)
        break

      await delay(process.env.RETRY);
    }
    return response
  }
  async RequestMaker(ServiceListRequest, params) {
    let stnParams
    let t1, t2
    switch (ServiceListRequest.type) {
      case 'OAuth1':
        return new Promise(async (resolve, reject) => {
          request.post(
            ServiceListRequest.url,
            {
              oauth: {
                consumer_key: ServiceListRequest.client_id,
                consumer_secret: ServiceListRequest.client_secret,
                realm: ServiceListRequest.app_id
              },
              form: params
            },
            async (err, res, body) => {
              var result = require('querystring').parse(body)
              if (!err) {
                resolve(ServiceListRequest.class.onResponse(result))
              } else {
                reject(err.code)
              }
            }
          )
        })

      default:
        switch (ServiceListRequest.requestType) {
          case 'form':
            stnParams = {
              url: ServiceListRequest.url,
              form: params,
              headers: ServiceListRequest.headers,
              timeout: ServiceListRequest.timeout,
              forever: true
            }
            break
          case 'form-data':
            stnParams = {
              url: ServiceListRequest.url,
              formData: params,
              headers: ServiceListRequest.headers,

              timeout: ServiceListRequest.timeout,
              forever: true
            }
            break
          case "form-file":
            stnParams = {
              url: ServiceListRequest.url,
              formData: params,
              headers: ServiceListRequest.headers,
              timeout: ServiceListRequest.timeout,
            };
            break;
          case 'raw':
            stnParams = {
              url: ServiceListRequest.url,
              body: JSON.stringify(params),
              headers: ServiceListRequest.headers,
              timeout: ServiceListRequest.timeout,
              forever: true
            }
            break
          case 'params':
            stnParams = {
              url: ServiceListRequest.url,
              body: JSON.stringify(params),
              rejectUnauthorized: false,
              headers: ServiceListRequest.headers,
              timeout: ServiceListRequest.timeout,
              forever: true,
              encoding: ServiceListRequest.encode
            }
            break
          case 'params-simple':
            stnParams = {
              url: ServiceListRequest.url,
              qs: params,
              rejectUnauthorized: false,
              headers: ServiceListRequest.headers,
              timeout: ServiceListRequest.timeout,
              forever: true,
              encoding: ServiceListRequest.encode
            }
            break
          default:
            stnParams = {}
            break
        }

        if (params !== null) {
          if (Object.keys(params).length == 0) {
            return {
              hasError: true,
              status: process.env[err.code]
            }
          }
        } else {
          return {
            hasError: true,
            status: process.env[err.code]
          }
        }

        t1 = new Date().getTime()
        switch (ServiceListRequest.method) {
          case 'post':
            return new Promise(async (resolve, reject) => {
              await request.post(stnParams, async (err, res, body) => {
                if (!err) {
                  t2 = new Date().getTime()
                  resolve({
                    statusCode: res.statusCode,
                    body: body
                  })
                } else {
                  // t2 = (new Date).getTime();
                  resolve({
                    hasError: true,
                    statusCode: 500,
                    status: process.env[err.code]
                  })
                }
              })
            })

          case 'get':
            return new Promise(async (resolve, reject) => {
              await request.get(stnParams, async (err, res, body) => {
                if (!err) {
                  t2 = new Date().getTime()
                  resolve({
                    statusCode: res.statusCode,
                    body: body
                  })
                } else {
                  t2 = new Date().getTime()
                  resolve({
                    hasError: true,
                    statusCode: 500,
                    status: process.env[err.code]
                  })
                }
              })
            })
          default:
            stnParams = {}
            break
        }
    }
  }
}
module.exports = new Request()