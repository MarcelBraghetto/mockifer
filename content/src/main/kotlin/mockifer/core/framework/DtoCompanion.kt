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

interface DtoCompanion<T> {
    /**
     * Given a data transfer object, provide the opportunity to sanitize it or
     * initialize anything, effectively 'reviving' it.
     */
    fun revive(dto: T) {
        // Stub by default
    }

    /**
     * Attempt to take a json string and convert it into the required dto type. During the
     * conversion, the dto will be revived.
     */
    fun fromJson(json: String?): T? {
        if (json == null) {
            return null
        }
        
        try {
            val dto = JSON.parse<T>(json)
            revive(dto)

            if (!isValid(dto)) {
                return null
            }

            return dto
        } catch (e: dynamic) {
            mockifer_log("DtoCompanion.fromJson: Caught 'dynamic' exception: unable to parse json object: $json")
            return null
        } catch (e: Error) {
            mockifer_log("DtoCompanion.fromJson: Caught 'error' exception ${e.message}: while parsing json object: $json")
            return null
        }
    }

    /**
     * Override this if a dto needs any special rules to be deemed 'valid'
     */
    fun isValid(dto: T): Boolean {
        return true
    }
}