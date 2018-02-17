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

#include "MockiferRoute.h"

USING_NS_MOCKIFER;

static const char* ROUTE_IS_INTERNAL = "isInternal";
static const char* ROUTE_ID = "routeId";
static const char* ROUTE_DISPLAY_NAME = "routeDisplayName";
static const char* ROUTE_DESCRIPTION = "routeDescription";
static const char* REQUEST_METHOD = "requestMethod";
static const char* REQUEST_URI = "requestUri";
static const char* REQUEST_OVERRIDE_ROUTE_ID = "requestOverrideRouteId";
static const char* REQUEST_QUERY_STRING_CONTAINS = "requestQueryStringContains";
static const char* REQUEST_QUERY_STRING_EQUALS = "requestQueryStringEquals";
static const char* REQUEST_BODY_JSON_PATH = "requestBodyJsonPath";
static const char* REQUEST_BODY_JSON_PATH_CONTAINS = "requestBodyJsonPathContains";
static const char* REQUEST_BODY_JSON_PATH_EQUALS = "requestBodyJsonPathEquals";
static const char* RESPONSE_STATUS_CODE = "responseStatusCode";
static const char* RESPONSE_DELAY = "responseDelay";
static const char* RESPONSE_JSON_FILE = "responseJsonFile";
static const char* RESPONSE_CONTROLLER_ID = "responseControllerId";

MockiferRoute::MockiferRoute() {
    isInternal = false;
    responseStatusCode = 0;
    responseDelay = 0;
}

bool MockiferRoute::populateFromJsonString(const string &jsonString) {
    Json::Value root;
    Json::Reader reader;
    
    bool parsingSuccessful = reader.parse(jsonString, root);
    if (!parsingSuccessful) {
        return false;
    }

    return populateFromJsonObject(root);
}

bool MockiferRoute::populateFromJsonObject(const Json::Value &jsonObject) {
    if (jsonObject.isMember(ROUTE_ID) && jsonObject[ROUTE_ID].isString()) {
        routeId = jsonObject[ROUTE_ID].asString();
    } else {
        // Must have a route ID at least.
        return false;
    }
    
    if (jsonObject.isMember(ROUTE_IS_INTERNAL) && jsonObject[ROUTE_IS_INTERNAL].isBool()) {
        isInternal = jsonObject[ROUTE_IS_INTERNAL].asBool();
    }
    
    if (jsonObject.isMember(ROUTE_DISPLAY_NAME) && jsonObject[ROUTE_DISPLAY_NAME].isString()) {
        routeDisplayName = jsonObject[ROUTE_DISPLAY_NAME].asString();
    }
    
    if (jsonObject.isMember(ROUTE_DESCRIPTION) && jsonObject[ROUTE_DESCRIPTION].isString()) {
        routeDescription = jsonObject[ROUTE_DESCRIPTION].asString();
    }
    
    if (jsonObject.isMember(REQUEST_METHOD) && jsonObject[REQUEST_METHOD].isString()) {
        requestMethod = jsonObject[REQUEST_METHOD].asString();
    }
    
    if (jsonObject.isMember(REQUEST_URI) && jsonObject[REQUEST_URI].isString()) {
        requestUri = jsonObject[REQUEST_URI].asString();
        uriRegex = regex(requestUri);
    }
    
    if (jsonObject.isMember(REQUEST_OVERRIDE_ROUTE_ID) && jsonObject[REQUEST_OVERRIDE_ROUTE_ID].isString()) {
        requestOverrideRouteId = jsonObject[REQUEST_OVERRIDE_ROUTE_ID].asString();
    }
    
    if (jsonObject.isMember(REQUEST_QUERY_STRING_CONTAINS) && jsonObject[REQUEST_QUERY_STRING_CONTAINS].isString()) {
        requestQueryStringContains = jsonObject[REQUEST_QUERY_STRING_CONTAINS].asString();
    }
    
    if (jsonObject.isMember(REQUEST_QUERY_STRING_EQUALS) && jsonObject[REQUEST_QUERY_STRING_EQUALS].isString()) {
        requestQueryStringEquals = jsonObject[REQUEST_QUERY_STRING_EQUALS].asString();
    }
    
    if (jsonObject.isMember(REQUEST_BODY_JSON_PATH) && jsonObject[REQUEST_BODY_JSON_PATH].isString()) {
        requestBodyJsonPath = jsonObject[REQUEST_BODY_JSON_PATH].asString();
    }
    
    if (jsonObject.isMember(REQUEST_BODY_JSON_PATH_CONTAINS) && jsonObject[REQUEST_BODY_JSON_PATH_CONTAINS].isString()) {
        requestBodyJsonPathContains = jsonObject[REQUEST_BODY_JSON_PATH_CONTAINS].asString();
    }
    
    if (jsonObject.isMember(REQUEST_BODY_JSON_PATH_EQUALS) && jsonObject[REQUEST_BODY_JSON_PATH_EQUALS].isString()) {
        requestBodyJsonPathEquals = jsonObject[REQUEST_BODY_JSON_PATH_EQUALS].asString();
    }
    
    if (jsonObject.isMember(RESPONSE_STATUS_CODE) && jsonObject[RESPONSE_STATUS_CODE].isUInt()) {
        responseStatusCode = jsonObject[RESPONSE_STATUS_CODE].asUInt();
    }

    if (jsonObject.isMember(RESPONSE_DELAY) && jsonObject[RESPONSE_DELAY].isUInt()) {
        responseDelay = jsonObject[RESPONSE_DELAY].asUInt();
    }
    
    if (jsonObject.isMember(RESPONSE_JSON_FILE) && jsonObject[RESPONSE_JSON_FILE].isString()) {
        responseJsonFile = jsonObject[RESPONSE_JSON_FILE].asString();
    }
    
    if (jsonObject.isMember(RESPONSE_CONTROLLER_ID) && jsonObject[RESPONSE_CONTROLLER_ID].isString()) {
        responseControllerId = jsonObject[RESPONSE_CONTROLLER_ID].asString();
    }

    return isValid();
}

Json::Value MockiferRoute::toJsonObject() {
    Json::Value json;
    
    json[ROUTE_IS_INTERNAL] = isInternal;
    
    if (responseStatusCode > 0) {
        json[RESPONSE_STATUS_CODE] = responseStatusCode;
    }
    
    if (responseDelay > 0) {
        json[RESPONSE_DELAY] = responseDelay;
    }
    
    if (!routeId.empty()) { json[ROUTE_ID] = routeId; }
    if (!routeDisplayName.empty()) { json[ROUTE_DISPLAY_NAME] = routeDisplayName; }
    if (!routeDescription.empty()) { json[ROUTE_DESCRIPTION] = routeDescription; }
    if (!requestMethod.empty()) { json[REQUEST_METHOD] = requestMethod; }
    if (!requestUri.empty()) { json[REQUEST_URI] = requestUri; }
    if (!requestOverrideRouteId.empty()) { json[REQUEST_OVERRIDE_ROUTE_ID] = requestOverrideRouteId; }
    if (!requestQueryStringContains.empty()) { json[REQUEST_QUERY_STRING_CONTAINS] = requestQueryStringContains; }
    if (!requestQueryStringEquals.empty()) { json[REQUEST_QUERY_STRING_EQUALS] = requestQueryStringEquals; }
    if (!requestBodyJsonPath.empty()) { json[REQUEST_BODY_JSON_PATH] = requestBodyJsonPath; }
    if (!requestBodyJsonPathContains.empty()) { json[REQUEST_BODY_JSON_PATH_CONTAINS] = requestBodyJsonPathContains; }
    if (!requestBodyJsonPathEquals.empty()) { json[REQUEST_BODY_JSON_PATH_EQUALS] = requestBodyJsonPathEquals; }
    if (!responseJsonFile.empty()) { json[RESPONSE_JSON_FILE] = responseJsonFile; }
    if (!responseControllerId.empty()) { json[RESPONSE_CONTROLLER_ID] = responseControllerId; }
    
    return json;
}

string MockiferRoute::toJson() {
    Json::StyledWriter writer;    
    return writer.write(toJsonObject());
}

uint MockiferRoute::getFilterCount() {
    uint numFilters = 0;
    
    if (!requestMethod.empty()) { numFilters++; }
    if (!requestUri.empty()) { numFilters++; }
    if (!requestQueryStringContains.empty()) { numFilters++; }
    if (!requestQueryStringEquals.empty()) { numFilters++; }
    if (!requestBodyJsonPathContains.empty()) { numFilters++; }
    if (!requestBodyJsonPathEquals.empty()) { numFilters++; }
    
    return numFilters;
}

bool MockiferRoute::isValid() {
    return !routeId.empty();
}
