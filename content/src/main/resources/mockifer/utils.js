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

/*
 Return a formatted long string of a date. Example: 2017-07-29T16:01:02.691+12:00
*/
function mockifer_getFormattedDateLong(date) {
    var shortDate = mockifer_getFormattedDateShort(date);
    var hours = mockifer_paddedNumberString(date.getHours(), 2);
    var minutes = mockifer_paddedNumberString(date.getMinutes(), 2);
    var seconds = mockifer_paddedNumberString(date.getSeconds(), 2);
    var milliseconds = mockifer_paddedNumberString(date.getMilliseconds(), 3);

    var timeZoneOffset = -date.getTimezoneOffset();
    var timeZoneSymbol = timeZoneOffset >= 0 ? '+' : '-';
    var timeZoneHours = mockifer_paddedNumberString(Math.abs(timeZoneOffset) / 60, 2);
    var timeZoneMinutes = mockifer_paddedNumberString(timeZoneOffset % 60, 2);

    return [shortDate, "T", hours, ":", minutes, ":", seconds, ".", milliseconds, timeZoneSymbol, timeZoneHours, ":", timeZoneMinutes].join('');
}

/*
 Return a formatted short string of a date. Example: 2017-07-29
*/
function mockifer_getFormattedDateShort(date) {
    var year = date.getFullYear();
    var month = mockifer_paddedNumberString(date.getMonth() + 1, 2);
    var day = mockifer_paddedNumberString(date.getDate(), 2);
    return [year, month, day].join('-');
}

/*
 Add (or subtract) a number of days from a given date.
*/
function mockifer_addDaysToDate(date, days) {
    var offsetInMilliseconds = days * 86400000;
    var updatedTime = date.getTime() + offsetInMilliseconds;
    return new Date(updatedTime);
}

/*
 Left pad a number up to the indicated number of times.
*/
function mockifer_paddedNumberString(number, padTo) {
    var result = "" + number;

    while (result.length < padTo) {
        result = "0" + result;
    }

    return result;
}
