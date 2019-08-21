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

#include <mutex>
#include <memory>
#include <set>
#include "MockiferCore.h"
#include "MockiferResponse.h"
#include "MockiferRoute.h"
#include "MockiferRouteManager.h"

NS_MOCKIFER_BEGIN

class MockiferServer {
    
public:
    // User methods
    static uint startOnDynamicPort(const string &contentPath);
    static void start(const string &contentPath, const string &port);
    static void startCustom(const string &contentPath, const int &customPort);
    static void stop();
    static void reset();
    static bool pushMock(const string &mockRouteId, const int &times);
    static bool clearActiveMocks();
    static string getBaseUrl();
    static void setCommandUrl(const string &commandUrl, const int &commandPort);
    
    // Administration
    static string sContentPath;
    static uint sPort;
    static uint sGlobalResponseDelay;
    static string sCommandUrl;
    static uint sCommandPort;
    unique_ptr<MockiferRouteManager> routesManager;
    static void addDataSyncPath(const string &syncPath);
    static bool updateExistingRoute(const string &routeJson, const string &jsonFileContent);
    static bool createNewRoute(const string &routeJson, const string &jsonFileContent);
    static bool deleteRoute(const string &routeId);
    static string loadRouteJsonFileContent(const string &routeId);
    static void setGlobalResponseDelay(uint delayInMillis);
    
private:
    struct mg_context *serverContext;
    struct mg_callbacks serverCallbacks;
    
    duk_context *jsContext;
    bool serverOk;
    set<string> binaryResponseFileTypes;
    
    static int handleRequest(struct mg_connection *connection);
    
    MockiferServer(const string &contentPath, const string &port);
    ~MockiferServer();
    bool isFileTypeRegistered(const string &fileType);
    void startServer();
    void resetServer();
    bool loadJavascriptFile(const string &fileName);
    static MockiferResponse processRequest(MockiferRequest request, bool logRequest);
    void sendMockiferCommand(const string &method, const string &uri, const string &body);
    MockiferResponse processMockiferCommand(MockiferRequest request);
    bool shouldSendCommandsAsRequests();
};

NS_MOCKIFER_END
