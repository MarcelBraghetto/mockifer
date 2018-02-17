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

#pragma once

#include "MockiferCore.h"

/*
 A request object has the following structure and will be passed into mockifer_glue.js/mockifer_handleServerRequest
 on every received http request:
 
 {
    "method: "GET|POST|PUT|HEAD",
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
 
 */

NS_MOCKIFER_BEGIN

struct MockiferRequest {
    
public:
    MockiferRequest(const struct mg_request_info *requestInfo, string requestBody);
    string createDto();
    string requestMethod;
    string requestUri;
    string requestQueryString;
    string requestBody;
    Json::Value requestBodyJson;
    
private:
    const struct mg_request_info *requestInfo;
};

NS_MOCKIFER_END
