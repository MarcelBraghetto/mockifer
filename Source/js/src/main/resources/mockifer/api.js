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

 Method: mockifer_handleServerRequest([Non null] request, [Nullable] route)

 This method is invoked by the core engine whenever a request is received.

 request:   represents the elements of the Http request that was received.
 route:     represents the route that was identified that should handle the request,
            or null if no route could be matched to the request.

 returns:   A valid 'response' json object should be returned which encapsulates what the
            request should be answered with.

 -------------------------------------------------------------
 INPUT - REQUEST, ROUTE:
 -------------------------------------------------------------
 
 REQUEST:

 A request object has the following structure:
 
 {
    "method: "NORMAL REST VERB eg GET",
    "uri": "/api/something",
    "queryString": "a=1&b=2",
    "headers": [
        {
            "key": "header key",
            "value": "header value"
        }
    ],
    "body": "request body"
 }
 
 Note: The following fields are optional and will not be present if the request did not include them:
 
 - 'queryString'
 - 'body'

 ROUTE:

 A route object has the following structure:

 {
     "routeId": "cats.getallcats,
     "routeIsStatic": true,
     "routeDisplayName": "Get all cats",
     "routeDescription": "Fetch all the cats in the current session",

     "requestMethod": "GET",
     "requestUri": "/cats",
     "requestOverrideRouteId": "internal.cats.getallcats",
     "requestQueryStringContains": "kittens",
     "requestQueryStringEquals": "kittens=true",

     "requestBodyJsonPath": "cats/0/name",
     "requestBodyJsonPathContains": "bob",
     "requestBodyJsonPathEquals": "bobcat",

     "responseStatusCode": 200,
     "responseJsonFile": "cats.getallcats.json",
     "responseControllerId": "internal.cats.getallcats"
  }

  Note: all fields except 'routeId' are optional and should be checked before use.

 -------------------------------------------------------------
 OUTPUT - RESPONSE:
 -------------------------------------------------------------
 
 This method MUST return a JSON string with the following structure or it will not be parsed by the Mockifer server.
 
 {
    "statusCode": 200,
    "headers": [
        {
            "key": "header key",
            "value": "header value"
        }
    ]
    "body": "response body"
 }
 
 */
function mockifer_handleServerRequest(requestJson, routeJson) {
    return mockifer.mockiferProcessRequest(requestJson, routeJson);
}

/*
 Reset the internal server session state to its defaults.
*/
function mockifer_reset() {
    mockifer.mockiferReset();
}

/*
 Loads and returns the text content of the given file name that is located relative to
 the mockifer_js folder. This method is only available when running within the Mockifer
 engine via Duktape.

 fileName:  name of the text file to load including its relative path within the content folder.

 returns:   the plain text content of the file if successful, or null if the file could not be loaded.
 */
function mockifer_loadDataFile(fileName) {
    if (duktape_loadDataFile) {
        return duktape_loadDataFile(fileName);
    } else {
        return null;
    }
}

/*
 Provide a way to log messages to the console output.

 message: to display.
*/
function mockifer_log(message) {
    if (duktape_printLine) {
        duktape_printLine(message);
    }
}

/*
 The host server can give a list of all the 'active' mocks that are currently running.
*/
function mockifer_getActiveMocks() {
    if (duktape_getActiveMocks) {
        return duktape_getActiveMocks();
    } else {
        return null;
    }
}