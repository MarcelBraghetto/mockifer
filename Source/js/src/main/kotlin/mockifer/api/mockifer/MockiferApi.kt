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

package mockifer.api.mockifer

import mockifer.core.ServerSession
import mockifer.core.framework.FileLoader
import mockifer.core.framework.mockifer_getActiveMocks
import mockifer.core.framework.toJson
import mockifer.core.model.*

object MockiferApi {

    class JsonFileMockResponseController(serverSession: ServerSession) : RequestController(serverSession) {
        override fun respondToRequest(route: Route, request: Request): Response {
            val headers = createResponseHeaders(request)

            val responseBody = FileLoader.loadRawDataFile("data/mocks/${route.responseJsonFile}") ?: return Response(
                    StatusCode.INTERNAL_SERVER_ERROR,
                    headers,
                    ErrorResponseBody("Route with id '${route.routeId}' was run, but the response json file ${route.responseJsonFile} failed to load.", serverSession).toJson())

            return Response(route.responseStatusCode, headers, responseBody)
        }
    }

    class HtmlExampleController(serverSession: ServerSession) : RequestController(serverSession) {
        override fun respondToRequest(route: Route, request: Request): Response {
            val content = FileLoader.loadRawDataFile("data/internal/html/example.html") ?: ""
            return Response(StatusCode.SUCCESS, createResponseHeaders(request), content)
        }
    }

    class GetAllRoutesController(serverSession: ServerSession) : RequestController(serverSession) {
        override fun respondToRequest(route: Route, request: Request): Response {
            val headers = createResponseHeaders(request)
            val responseBody = FileLoader.loadRawDataFile("routes.json") ?: return Response(
                    StatusCode.INTERNAL_SERVER_ERROR,
                    headers,
                    ErrorResponseBody("GetAllRoutesController failed to load 'routes.json'", serverSession).toJson())

            return Response(route.responseStatusCode, headers, responseBody)
        }
    }

    class GetAllActiveMocks(serverSession: ServerSession) : RequestController(serverSession) {
        override fun respondToRequest(route: Route, request: Request): Response {
            val headers = createResponseHeaders(request)
            val responseBody = mockifer_getActiveMocks() ?: return Response(
                    StatusCode.INTERNAL_SERVER_ERROR,
                    headers,
                    ErrorResponseBody("GetAllActiveMocks failed to load fetch active mocks from host.", serverSession).toJson())

            return Response(route.responseStatusCode, headers, responseBody)
        }
    }
}
