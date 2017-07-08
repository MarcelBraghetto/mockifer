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

package mockifer.core

import mockifer.api.mockifer.MockiferApi
import mockifer.core.framework.isMissingOrEmpty
import mockifer.core.framework.toJson
import mockifer.core.model.ErrorResponseBody
import mockifer.core.model.Request
import mockifer.core.model.Response
import mockifer.core.model.Route
import mockifer.core.model.StatusCode.BAD_REQUEST
import mockifer.core.model.StatusCode.INTERNAL_SERVER_ERROR
import mockifer.core.model.StatusCode.NOT_FOUND_ERROR

object Server {
    private lateinit var serverSession: ServerSession

    init {
        reset()
    }

    /**
     * Reset the server session to its initial state.
     */
    fun reset() {
        serverSession = ServerSession()
    }

    fun processRequest(requestJson: String, routeJson: String?): String {
        // Try to parse the request
        val request = Request.fromJson(requestJson) ?:
                return Response(
                        BAD_REQUEST,
                        serverSession.createResponseHeaders(),
                        ErrorResponseBody("Malformed request received.", serverSession).toJson()).toJson()

        // Try to parse the route action to evaluate.
        val route = Route.fromJson(routeJson) ?:
                return Response(
                        NOT_FOUND_ERROR,
                        serverSession.createResponseHeaders(),
                        ErrorResponseBody("No route found for request.", serverSession).toJson()).toJson()

        // If the route specifies to return the body of a json data file then process it that way.
        if (!route.responseJsonFile.isMissingOrEmpty()) {
            return MockiferApi.JsonFileMockResponseController(serverSession).respondToRequest(route, request).toJson()
        }

        // If the route specifies to execute an internal handler, trigger it to run.
        if (!route.responseControllerId.isMissingOrEmpty()) {
            serverSession.createRequestController(route.responseControllerId)?.let {
                return it.respondToRequest(route, request).toJson()
            }
        }

        // If we reach this point then we found a matching route but there was a problem with
        // the way it was parsed that caused it to fail to return a json blob or run a controller.
        return Response(
                INTERNAL_SERVER_ERROR,
                serverSession.createResponseHeaders(request),
                ErrorResponseBody("Route ${route.routeId} was found but could not be successfully executed.", serverSession).toJson()).toJson()
    }
}