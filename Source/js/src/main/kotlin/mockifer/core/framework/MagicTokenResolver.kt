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

import kotlin.js.Date

object MagicTokenResolver {
    // A 'magic token' looks like: {{token_id}}
    private val magicTokenPattern = """\{\{([^}]*?)}}""".toRegex()

    /**
     * Given an input string, scan and replace any 'magic tokens' if they can be
     * resolved to a dynamic substitution.
     */
    fun resolveMagicTokens(input: String): String {
        return input.replace(magicTokenPattern, { resolveToken(it.value) })
    }

    /**
     * When a 'magic token' is detected in a string, it is 'resolved' by looking
     * for a match to its content and replaced with whatever string is appropriate,
     * or if the content is unknown, the token is returned unchanged.
     *
     * @param magicToken that was found in the shape of {{content}}
     * @return a substitution for the magic token content if one could be found.
     */
    private fun resolveToken(magicToken: String): String {
        // Chop the leading and trailing double braces, leaving the content
        val content = magicToken.substring(2, magicToken.length - 2)

        // Handle any magic dates
        if (content.startsWith("today")) {
            return resolveToday(content)
        }

        return magicToken
    }

    private fun resolveToday(content: String): String {
        var date = Date()
        date = adjustDateForOperator(date, content, "-")
        date = adjustDateForOperator(date, content, "+")

        if (content.startsWith("todayTime")) {
            return date.toLongDateString()
        }

        return date.toShortDateString()
    }

    private fun adjustDateForOperator(date: Date, content: String, operator: String): Date {
        val operatorIndex = content.lastIndexOf(operator)
        if (operatorIndex > 0) {
            content.substring(operatorIndex, content.length).toIntOrNull()?.let {
                return date.addDays(it)
            }
        }

        return date
    }
}
