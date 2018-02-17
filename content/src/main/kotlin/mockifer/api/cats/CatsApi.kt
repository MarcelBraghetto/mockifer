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

package mockifer.api.cats

import mockifer.core.ServerSession
import mockifer.core.framework.getFirstMatch
import mockifer.core.framework.toJson
import mockifer.core.model.*
import mockifer.core.model.StatusCode.NOT_FOUND_ERROR
import mockifer.core.model.StatusCode.SUCCESS

object CatsApi {

    @Suppress("unused")
    class GetAllCatsController(serverSession: ServerSession) : RequestController(serverSession) {
        override fun respondToRequest(route: Route, request: Request): Response {
            return Response(SUCCESS, createResponseHeaders(request), object : ResponseBody(serverSession) {
                @JsName("cats") val cats = serverSession.catsRepository.getAllCats()
            }.toJson())
        }
    }

    @Suppress("unused")
    class CatDetailsController(serverSession: ServerSession) : RequestController(serverSession) {
        override fun respondToRequest(route: Route, request: Request): Response {
            val responseHeaders = createResponseHeaders(request)
            val id = route.requestUri.toRegex().getFirstMatch(request.uri)

            val cat = serverSession.catsRepository.getCat(id) ?: return Response(
                    NOT_FOUND_ERROR,
                    responseHeaders,
                    ErrorResponseBody("Cat with id $id not found.", serverSession).toJson())

            return Response(SUCCESS, responseHeaders, object : ResponseBody(serverSession) {
                @JsName("cat") val cat = cat
            }.toJson())
        }
    }

    class CreateCatController(serverSession: ServerSession) : RequestController(serverSession) {
        override fun respondToRequest(route: Route, request: Request): Response {
            val responseHeaders = createResponseHeaders(request)

            val cat = Cat.fromJson(request.body) ?: return Response(
                    StatusCode.BAD_REQUEST,
                    responseHeaders,
                    ErrorResponseBody("Unable to parse request body: ${request.body}", serverSession).toJson())

            return Response(SUCCESS, responseHeaders, object : ResponseBody(serverSession) {
                @Suppress("unused") @JsName("cat") val cat = serverSession.catsRepository.createCat(cat)
            }.toJson())
        }
    }

    class DeleteCatController(serverSession: ServerSession) : RequestController(serverSession) {
        override fun respondToRequest(route: Route, request: Request): Response {
            val responseHeaders = createResponseHeaders(request)
            val id = route.requestUri.toRegex().getFirstMatch(request.uri)

            if (serverSession.catsRepository.deleteCat(id)) {
                return Response(SUCCESS, responseHeaders, ResponseBody(serverSession).toJson())
            }

            return Response(NOT_FOUND_ERROR, responseHeaders, ErrorResponseBody("Cat with id $id not found.", serverSession).toJson())
        }
    }

    @Suppress("unused")
    class UpdateCatController(serverSession: ServerSession) : RequestController(serverSession) {
        override fun respondToRequest(route: Route, request: Request): Response {
            val responseHeaders = createResponseHeaders(request)

            val cat = Cat.fromJson(request.body) ?: return Response(
                    StatusCode.BAD_REQUEST,
                    responseHeaders,
                    ErrorResponseBody("Cannot update cat - unable to parse json: ${request.body}", serverSession).toJson())

            val result = serverSession.catsRepository.updateCat(cat) ?: return Response(
                    NOT_FOUND_ERROR,
                    responseHeaders,
                    ErrorResponseBody("Cat to update was not found.", serverSession).toJson())

            return Response(SUCCESS, responseHeaders, object : ResponseBody(serverSession) {
                @JsName("originalCat") val originalCat = result.first
                @JsName("newCat") val newCat = result.second
            }.toJson())
        }
    }
}
