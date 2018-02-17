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

#include "MockiferRequest.h"

USING_NS_MOCKIFER;

MockiferRequest::MockiferRequest(const struct mg_request_info *requestInfo, string requestBody) {
    this->requestInfo = requestInfo;
    this->requestMethod = string(requestInfo->request_method);
    this->requestUri = string(requestInfo->request_uri);
    
    if (requestInfo->query_string) {
        this->requestQueryString = string(requestInfo->query_string);
    } else {
        this->requestQueryString = "";
    }
    
    this->requestBody = requestBody;
    
    Json::Value requestBodyJson;
    Json::Reader reader;
    
    bool parsingSuccessful = reader.parse(this->requestBody, requestBodyJson);
    if (parsingSuccessful) {
        this->requestBodyJson = requestBodyJson;
    }
}

string MockiferRequest::createDto() {
    Json::StyledWriter writer;
    Json::Value root;
    
    root["method"] = requestMethod;
    root["uri"] = requestUri;
    
    if (!requestQueryString.empty()) {
        root["queryString"] = requestQueryString;
    }

    Json::Value headers(Json::arrayValue);
    for (int i = 0; i < requestInfo->num_headers; i++) {
        auto header = requestInfo->http_headers[i];
        Json::Value jsonHeader;
        jsonHeader["key"] = string(header.name);
        jsonHeader["value"] = string(header.value);
        headers.append(jsonHeader);
    }
    root["headers"] = headers;
    
    if (requestBody != "") {
        root["body"] = requestBody;
    }

    return writer.write(root);
}
