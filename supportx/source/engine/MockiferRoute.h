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

#include <vector>
#include <regex>
#include "MockiferCore.h"

/*
 A route is represented by the following JSON structure:
 
 {
    "routeId": "cats.getallcats,
    "isInternal": true,
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
    "responseDelay": 0,
    "responseJsonFile": "cats.getallcats.json",
    "responseControllerId": "cats.getallcats"
 }
 
 */

NS_MOCKIFER_BEGIN

struct MockiferRoute {
    
public:
    string routeId;
    bool isInternal;
    string routeDisplayName;
    string routeDescription;
    
    string requestMethod;
    string requestUri;
    string requestOverrideRouteId;
    string requestQueryStringContains;
    string requestQueryStringEquals;
    
    string requestBodyJsonPath;
    string requestBodyJsonPathContains;
    string requestBodyJsonPathEquals;
    
    uint responseStatusCode;
    uint responseDelay;
    string responseJsonFile;
    string responseControllerId;
    
    regex uriRegex;
    
    MockiferRoute();
    bool populateFromJsonString(const string &jsonString);
    bool populateFromJsonObject(const Json::Value &jsonObject);
    
    string toJson();
    Json::Value toJsonObject();
    
    uint getFilterCount();
    
    bool isValid();
};

NS_MOCKIFER_END
