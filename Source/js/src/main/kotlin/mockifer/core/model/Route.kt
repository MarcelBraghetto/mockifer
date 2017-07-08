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

class Route(@JsName("routeId") var routeId: String,
            @JsName("isInternal") var isInternal: Boolean,
            @JsName("routeDisplayName") var routeDisplayName: String,
            @JsName("routeDescription") var routeDescription: String,
            @JsName("requestMethod") var requestMethod: String,
            @JsName("requestUri") var requestUri: String,
            @JsName("requestOverrideRouteId") var requestOverrideRouteId: String,
            @JsName("requestQueryStringContains") var requestQueryStringContains: String,
            @JsName("requestQueryStringEquals") var requestQueryStringEquals: String,
            @JsName("requestBodyJsonPath") var requestBodyJsonPath: String,
            @JsName("requestBodyJsonPathContains") var requestBodyJsonPathContains: String,
            @JsName("requestBodyJsonPathEquals") var requestBodyJsonPathEquals: String,
            @JsName("responseStatusCode") var responseStatusCode: Int,
            @JsName("responseJsonFile") var responseJsonFile: String,
            @JsName("responseControllerId") var responseControllerId: String) {

    companion object : DtoCompanion<Route> {
        override fun revive(dto: Route) {
            with (dto) {
                routeId = routeId.orDefault("")
                isInternal = isInternal.orDefault(false)
                routeDisplayName = routeDisplayName.orDefault("")
                routeDescription = routeDescription.orDefault("")
                requestMethod = requestMethod.orDefault("")
                requestUri = requestUri.orDefault("")
                requestOverrideRouteId = requestOverrideRouteId.orDefault("")
                requestQueryStringContains = requestQueryStringContains.orDefault("")
                requestQueryStringEquals = requestQueryStringEquals.orDefault("")
                requestBodyJsonPath = requestBodyJsonPath.orDefault("")
                requestBodyJsonPathContains = requestBodyJsonPathContains.orDefault("")
                requestBodyJsonPathEquals = requestBodyJsonPathEquals.orDefault("")
                responseStatusCode = responseStatusCode.orDefault(200)
                responseJsonFile = responseJsonFile.orDefault("")
                responseControllerId = responseControllerId.orDefault("")
            }
        }
    }
}
