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

/**
 * Convert an object into a json string representation.
 */
fun Any.toJson(): String = JSON.stringify(this, { _, v -> v }, 4)

/**
 * Determine if the object is null or undefined.
 */
fun Any?.isMissing(): Boolean = this == null || this == undefined

/**
 * Determine if a string is null, undefined or empty.
 */
fun String?.isMissingOrEmpty(): Boolean = this.isMissing() || this?.length == 0

/**
 * Given an object, apply some default value to it if it is null or undefined.
 */
fun <T> T?.orDefault(default: T): T = if (this.isMissing()) default else this!!

/**
 * Get the first match within a regex result. Useful for extracting segments from a string.
 */
fun Regex.getFirstMatch(input: String): String = this.find(input)?.destructured?.component1() ?: ""

/**
 * Get a short formatted string of the given date.
 */
fun Date.toShortDateString(): String = mockifer_getFormattedDateShort(this)

/**
 * Get a long formatted string of the given date.
 */
fun Date.toLongDateString(): String = mockifer_getFormattedDateLong(this)

/**
 * Add (or subtract) the given number of days to the given date.
 */
fun Date.addDays(days: Int): Date = mockifer_addDaysToDate(this, days)
