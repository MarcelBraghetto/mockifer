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

#include <chrono>
#include <thread>
#include "MockiferServer.h"
#include "MockiferRequest.h"
#include "MockiferUtil.h"

USING_NS_MOCKIFER;

using namespace std::chrono;
typedef std::chrono::high_resolution_clock Time;

static MockiferServer* sServer = nullptr;

static string LINE_BREAK = "\r\n";

#pragma mark Duktape Javascript VM interaction

static duk_ret_t duktape_printLine(duk_context *context) {
    string message = duk_get_string(context, -1); /* -1 = top of stack */
    LOGD(" ↳ Kotlin: %s", message.c_str());
    return 1;
}

static duk_ret_t duktape_getActiveMocks(duk_context *context) {
    if (!sServer) {
        LOGE("MockiferServer::duktape_getActiveMocks: Server offline - could not load fetch active mocks.");
        duk_push_string(context, NULL);
        return 1;
    }

    duk_push_string(context, sServer->routesManager->getAllActiveMocksAsJson().c_str());
    return 1;
}

static duk_ret_t duktape_loadDataFile(duk_context *context) {
    string fileName = duk_get_string(context, -1); /* -1 = top of stack */
    
    if (!sServer) {
        LOGE("MockiferServer::duktape_loadDataFile: Server offline - could not load requested data file: %s", fileName.c_str());
        duk_push_string(context, NULL);
        return 1;
    }

    // Attempt to load the given file and return its content.
    fileName = sServer->contentPath + "/" + fileName;
    auto loadResult = MockiferUtil::loadTextFile(fileName);
    if (loadResult.first == true) {
        duk_push_string(context, loadResult.second.c_str());
        return 1;
    }
    
    LOGE("MockiferServer::duktape_loadDataFile: Could not load requested data file: %s", fileName.c_str());
    duk_push_string(context, NULL);
    return 1; /* 1 = success, 0 = undefined, -1 = error */
}

#pragma mark Server lifecycle implementation

MockiferServer::MockiferServer(const string &contentPath, const string &port) {    
    this->contentPath = MockiferUtil::absolutePath(contentPath);
    this->port = port;
    this->routesManager = unique_ptr<MockiferRouteManager>(new MockiferRouteManager(contentPath));
    serverOk = false;
    globalResponseDelay = 0;
}

MockiferServer::~MockiferServer() {
    LOGD("Destroying server");
    serverOk = false;
    duk_destroy_heap(jsContext);
    jsContext = nullptr;
    
    mg_stop(serverContext);
    serverContext = nullptr;
}

void MockiferServer::setGlobalResponseDelay(uint delayInMillis) {
    if (sServer) {
        LOGD("Setting global response delay in milliseconds to %d", delayInMillis);
        sServer->globalResponseDelay = delayInMillis;
    }
}

void MockiferServer::start(const string &contentPath, const string &port) {
    if (sServer) {
        return;
    }

    sServer = new MockiferServer(contentPath, port);
    sServer->startServer();
}

void MockiferServer::stop() {
    LOGD("Stopping server");
    if (sServer) {
        sServer->serverOk = false;
        delete sServer;
        sServer = nullptr;
    }
}

void MockiferServer::reset() {
    LOGD("Resetting server");
    
    if (!sServer) {
        LOGE("Unable to reset - server was not running!");
        return;
    }
    
    sServer->resetServer();
}

void MockiferServer::resetServer() {
    globalResponseDelay = 0;
    routesManager->reload();
    duk_get_global_string(jsContext, "mockifer_reset");
    duk_call(jsContext, 0);
}

void MockiferServer::startServer() {
    LOGD("Booting Javascript Virtual Machine, be patient!");
    
    serverOk = true;
    
    // Preload all our routes
    routesManager->reload();
    
    // Start Duktape Javascript
    jsContext = duk_create_heap_default();
    
    // Register the 'duktape_loadDataFile' method to Javascript.
    duk_push_global_object(jsContext);
    duk_push_c_function(jsContext, duktape_loadDataFile, 1);
    duk_put_prop_string(jsContext, -2 /* -2 = global*/, "duktape_loadDataFile");
    duk_pop(jsContext);
    
    // Register the 'duktape_printLine' method to Javascript.
    duk_push_global_object(jsContext);
    duk_push_c_function(jsContext, duktape_printLine, 1);
    duk_put_prop_string(jsContext, -2 /* -2 = global*/, "duktape_printLine");
    duk_pop(jsContext);
    
    // Register the 'duktape_getActiveMocks' method to Javascript.
    duk_push_global_object(jsContext);
    duk_push_c_function(jsContext, duktape_getActiveMocks, 1);
    duk_put_prop_string(jsContext, -2 /* -2 = global*/, "duktape_getActiveMocks");
    duk_pop(jsContext);
    
    // Load the manifest
    auto manifest = MockiferUtil::loadManifest(contentPath);
    
    // Load the list of javascript files that need to bootstrap to start the server.
    if (manifest.javascriptBootstrapFiles.size() == 0) {
        serverOk = false;
    }
    
    for (const auto &javascriptFileName : manifest.javascriptBootstrapFiles) {
        if(!loadJavascriptFile(javascriptFileName)) {
            serverOk = false;
        }
    }
    
    // Load and register any binary file type suffixes to resolve for requests.
    for (const auto &fileType : manifest.binaryResponseFileTypes) {
        binaryResponseFileTypes.insert(fileType);
    }
    
    // Start the CivitWeb server - we deliberately limit the number of worker threads
    // to 1 so we can achieve a constant ordering of processing the request/response queue.
    string listenOn = "127.0.0.1:" + port;
    const char *options[] = {
        "listening_ports", listenOn.c_str(),
        "num_threads", "1",
        NULL
    };
    
    memset(&serverCallbacks, 0, sizeof(serverCallbacks));
    serverCallbacks.begin_request = handleRequest;
    
    LOGD("Starting HTTP server on port: %s", port.c_str());
    serverContext = mg_start(&serverCallbacks, NULL, options);
    
    if (serverContext == NULL) {
        LOGE("FATAL: Unable to start listening on port %s! Check that nothing else is already using that port.", port.c_str());
    }
}

string MockiferServer::getBaseUrl() {
    if (!sServer) {
        return "";
    }
    
    return "http://localhost:" + sServer->port;
}

#pragma mark Server support implementation

void MockiferServer::addDataSyncPath(const string &syncPath) {
    if (!sServer) {
        return;
    }
    
    sServer->routesManager->addDataSyncPath(syncPath);    
}

bool MockiferServer::loadJavascriptFile(const string &fileName) {
    if (!jsContext) {
        return false;
    }
    
    auto filePath = contentPath + "/" + fileName;
    auto loadResult = MockiferUtil::loadTextFile(filePath);
    
    if (loadResult.first == true) {
        duk_push_string(jsContext, loadResult.second.c_str());
        duk_eval(jsContext);
        duk_pop(jsContext);
        return true;
    }
    
    LOGE("FATAL: unable to load bootstrap Javascript file: %s", filePath.c_str());
    return false;
}

bool MockiferServer::isFileTypeRegistered(const string &fileType) {
    return binaryResponseFileTypes.find(fileType) != binaryResponseFileTypes.end();
}

int MockiferServer::handleRequest(struct mg_connection *connection) {
    if (!sServer || !sServer->serverOk) {
        LOGE("MockiferServer::handleRequest: server is unavailable before processing request.");
        return 0;
    }
    
    auto timeBefore = Time::now();
    const struct mg_request_info *requestInfo = mg_get_request_info(connection);
    
    string requestBody{""};
    auto requestBodyLength = requestInfo->content_length;
    if (requestBodyLength > 0) {
        char buf[requestBodyLength + 1];
        mg_read(connection, buf, requestBodyLength);
        buf[requestBodyLength] = '\0';
        requestBody = string(buf);
    }
    
    // Parse the request into a request object
    MockiferRequest request {requestInfo, requestBody};
    
    LOGD("Request: [%s] %s", request.requestMethod.c_str(), request.requestUri.c_str());
    
    // Check if this request is asking for a file whose extension is known to us.
    auto uri = MockiferUtil::toLowerCase(request.requestUri);
    auto fileSuffixIndex = uri.find_last_of(".");
    if (fileSuffixIndex != string::npos) {
        auto fileSuffix = uri.substr(fileSuffixIndex, uri.length());
        
        // See if its a suffix we recognise as a 'file' response.
        if (sServer->isFileTypeRegistered(fileSuffix)) {
            auto filePath = sServer->contentPath + requestInfo->local_uri;
            mg_send_file(connection, filePath.c_str());
            
            auto elapsed = duration_cast<milliseconds>(Time::now() - timeBefore);
            LOGD(" ↳ Responded in %ld ms with binary file %s", (long) elapsed.count(), requestInfo->local_uri);
            
            return 1;
        }
    }
    
    auto response = processRequest(request);
    
    if (!sServer || !sServer->serverOk) {
        LOGE("MockiferServer::handleRequest: server is unavailable after processing request.");
        return 0;
    }
    
    auto body = response.responseBody.c_str();
    
    ostringstream responseBody;
    
    // Response line
    responseBody << "HTTP/1.1 " << response.statusCode << LINE_BREAK;
    
    // Headers
    responseBody << "Content-Type: application/json" << LINE_BREAK;
    responseBody << "Content-Length: " << response.responseBodyLength << LINE_BREAK;

    // Other headers
    for (auto const& header : response.responseHeaders) {
        responseBody << header.first << ": " << header.second << LINE_BREAK;
    }
    
    // Response body
    responseBody << LINE_BREAK << body;
    
    mg_printf(connection, "%s", responseBody.str().c_str());
    
    auto elapsed = duration_cast<milliseconds>(Time::now() - timeBefore);
    LOGD(" ↳ Responded in %ld ms with statusCode %d", (long) elapsed.count(), response.statusCode);
    
    return 1; // returning 1 indicates we have sent everything to the client.
}

MockiferResponse MockiferServer::processRequest(MockiferRequest request) {
    auto context = sServer->jsContext;
    
    // Attempt to find a matching route that can handle the request
    auto route = sServer->routesManager->findMatchingRoute(request);
    
    auto requestDto = request.createDto();

    duk_get_global_string(context, "mockifer_handleServerRequest");
    duk_push_string(context, requestDto.c_str());
    duk_push_string(context, route.isValid() ? route.toJson().c_str() : NULL);
    duk_call(context, 2);
    MockiferResponse response {duk_safe_to_string(context, -1)};
    duk_pop(context);

    // Simulate a response delay based on whether the current route had a custom delay ...
    auto responseDelay = route.responseDelay;
    if (responseDelay == 0) {
        // or if not, adopt the global response delay of the server.
        responseDelay = sServer->globalResponseDelay;
    }
    
    // If there is a delay to respect, simulate it. This works because requests are handled
    // on a single worker thread so it will just make the next request wait its turn.
    if (responseDelay > 0) {
        this_thread::sleep_for(chrono::milliseconds(responseDelay));
    }
    
    return response;
}

#pragma mark Routes manager interaction

bool MockiferServer::pushMock(const string &mockRouteId, const int &times) {
    if (sServer) {
        return sServer->routesManager->pushMock(mockRouteId, times);
    }
    
    return false;
}

bool MockiferServer::clearActiveMocks() {
    if (sServer) {
        sServer->routesManager->clearActiveMocks();
        return true;
    }
    
    return false;
}

bool MockiferServer::updateExistingRoute(const string &routeJson, const string &jsonFileContent) {
    if (sServer) {
        return sServer->routesManager->updateExistingRoute(routeJson, jsonFileContent);
    }
    
    return false;
}

bool MockiferServer::createNewRoute(const string &routeJson, const string &jsonFileContent) {
    if (sServer) {
        return sServer->routesManager->createNewRoute(routeJson, jsonFileContent);
    }
    
    return false;
}

bool MockiferServer::deleteRoute(const string &routeId) {
    if (sServer) {
        return sServer->routesManager->deleteRoute(routeId);
    }
    
    return false;
}

string MockiferServer::loadRouteJsonFileContent(const string &routeId) {
    if (sServer) {
        return sServer->routesManager->loadMockDataFile(routeId);
    }
    
    return "";
}

// End: Route manage interaction
