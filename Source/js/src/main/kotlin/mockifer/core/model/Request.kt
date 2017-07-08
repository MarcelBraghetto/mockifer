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

/**
 * {
 *      "method: "GET|POST|PUT|HEAD|PATCH",
 *      "uri": "/api/something",
 *      "queryString": "a=1&b=2",
 *      "headers": [
 *      {
 *          "key": "header key",
 *          "value": "header value"
 *      }
 *      ],
 *      "body": "request body"
 * }
 *
 */
class Request(@JsName("method") var method: String,
              @JsName("uri") var uri: String,
              @JsName("headers") var headers: Array<Header>,
              @JsName("queryString") var queryString: String,
              @JsName("body") var body: String) {

    val queryParams: Map<String, String>

    init {
        val map = mutableMapOf<String, String>()

        queryString.split("&").forEach {
            val pair = it.split("=")

            if (pair.size == 2) {
                map[pair[0]] = pair[1]
            }
        }

        queryParams = map.toMap()
    }

    companion object : DtoCompanion<Request> {
        override fun revive(dto: Request) {
            with(dto) {
                method = method.orDefault("")
                uri = uri.orDefault("")
                headers = headers.orDefault(arrayOf())
                queryString = queryString.orDefault("")
                body = body.orDefault("")
            }
        }

        override fun isValid(dto: Request): Boolean {
            with(dto) {
                return method.isNotEmpty() && uri.isNotEmpty()
            }
        }
    }
}
