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

#include <map>
#include "MockiferCore.h"

/*
 The response dto returned from Javascript must have this *exact* structure,
 or Mockifer will return a generic 500 error instead.
 
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

NS_MOCKIFER_BEGIN

struct MockiferResponse {
    
public:
    int statusCode;
    unsigned long responseBodyLength;
    string responseBody;
    map<string, string> responseHeaders;
    
    void populateFromJson(string json);
    MockiferResponse();
    MockiferResponse(string responseDto);
};

NS_MOCKIFER_END
