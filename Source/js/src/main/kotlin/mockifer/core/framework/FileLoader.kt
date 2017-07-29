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

package mockifer.core.framework

import mockifer.core.framework.MagicTokenResolver.resolveMagicTokens

object FileLoader {
    /**
     * Attempt to load and parse the given file path into the given type.
     *
     * The parsing will be performed blindly, the onus is on the caller of this method to
     * perform any required validation after the content is loaded.
     *
     * Any 'magic tokens' will also be replaced with their substitutions during the load.
     */
    fun <T> loadJsonDataFile(filePath: String): T? {
        try {
            val json = resolveMagicTokens(mockifer_loadDataFile(filePath) ?: "")
            return JSON.parse<T>(json)
        } catch (e: dynamic) {
            return null
        } catch (e: Error) {
            return null
        }
    }

    /**
     * Load a raw string file without attempting to parse it.
     */
    fun loadRawDataFile(filePath: String): String? {
        try {
            return resolveMagicTokens(mockifer_loadDataFile(filePath) ?: "")
        } catch (e: dynamic) {
            return null
        } catch (e: Error) {
            return null
        }
    }
}
