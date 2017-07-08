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

#include <memory>
#include <vector>
#include "MockiferCore.h"
#include "MockiferRoute.h"
#include "MockiferRequest.h"

NS_MOCKIFER_BEGIN

struct MockiferRouteManager {
    
public:
    MockiferRouteManager(const string &sourceContentPath);
    
    bool reload();
    string getAllRoutesAsJson();
    string getAllActiveMocksAsJson();
    MockiferRoute findMatchingRoute(const MockiferRequest &request);
    void clearActiveMocks();
    
    bool pushMock(const string &mockRouteId, const int &times);
    bool updateExistingRoute(const string &routeJson, const string &jsonFileContent);
    bool createNewRoute(const string &routeJson, const string &jsonFileContent);
    bool deleteRoute(const string &routeId);
    string loadMockDataFile(const string &routeId);
    void addDataSyncPath(const string &syncPath);

private:
    string sourceContentPath;
    vector<string> dataSyncPaths;
    vector<MockiferRoute> internalRoutes;
    vector<MockiferRoute> activeMockRoutes;
    vector<MockiferRoute> allRoutes;
    
    bool isMatch(const MockiferRoute &route, const MockiferRequest &request);
    MockiferRoute consumeActiveMock(const MockiferRequest &request);
    MockiferRoute consumeActiveMock(const MockiferRoute &route);
    bool saveAndReload();
    bool deleteMockDataFile(const string &routeId);
    bool saveMockDataFile(const string &routeId, const string &content);
    
    pair<bool, string> crawlRequestBodyJsonForValue(const string &jsonPath, const Json::Value &jsonRoot);
};

NS_MOCKIFER_END
