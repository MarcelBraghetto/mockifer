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

import mockifer.api.cats.CatsApi
import mockifer.api.cats.CatsRepository
import mockifer.api.mockifer.MockiferApi
import mockifer.core.framework.toLongDateString
import mockifer.core.model.Header
import mockifer.core.model.Request
import mockifer.core.model.RequestController
import kotlin.js.Date

class ServerSession(val catsRepository: CatsRepository = CatsRepository()) {

    val formattedServerTime get() = Date().toLongDateString()

    fun createResponseHeaders(request: Request? = null): Array<Header> {
        val responseHeaders = mutableMapOf<String, String>()
        responseHeaders[Header.HEADER_CONNECTION] = Header.VALUE_CLOSE

        // Add any response headers that should be returned.
        responseHeaders["Mockifer"] = "Powered by Mockifer!"

        return responseHeaders.entries.map {
            Header(it.key, it.value)
        }.toTypedArray()
    }

    fun createRequestController(controllerId: String): RequestController? {
        when (controllerId.toLowerCase()) {

            // Mockifer API
            "mockifer.getallroutes" -> return MockiferApi.GetAllRoutesController(this)
            "mockifer.htmlexample" -> return MockiferApi.HtmlExampleController(this)
            "mockifer.getactivemocks" -> return MockiferApi.GetAllActiveMocks(this)

            // Cats API
            "cats.getallcats" -> return CatsApi.GetAllCatsController(this)
            "cats.catdetails" -> return CatsApi.CatDetailsController(this)
            "cats.createcat" -> return CatsApi.CreateCatController(this)
            "cats.updatecat" -> return CatsApi.UpdateCatController(this)
            "cats.deletecat" -> return CatsApi.DeleteCatController(this)

            // Nup, dunno what controller you want ...
            else -> return null
        }
    }
}
