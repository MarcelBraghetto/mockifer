/*
 * =================================================
 * Mockifer License
 * =================================================
 *
 * MIT License
 *
 * Copyright (c) 2017 Marcel Braghetto
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

package mockifer.core.model

import mockifer.core.framework.DtoCompanion
import mockifer.core.framework.orDefault
import mockifer.core.model.StatusCode.BAD_REQUEST

/**
 * {
 *      "statusCode": 200,
 *      "headers": [
 *          {
 *              "key": "header key",
 *              "value": "header value"
 *          }
 *      ]
 *      "body": "response body"
 * }
 */

class Response(@JsName("statusCode") var statusCode: Int,
               @JsName("headers") var headers: Array<Header>,
               @JsName("body") var body: String) {

    companion object : DtoCompanion<Response> {
        override fun revive(dto: Response) {
            with(dto) {
                statusCode = statusCode.orDefault(BAD_REQUEST)
                headers = headers.orDefault(arrayOf())
                body = body.orDefault("{}")
            }
        }
    }
}
