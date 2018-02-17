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
#include <random>
#include <algorithm>
#include "MockiferServer.h"
#include "MockiferRequest.h"
#include "MockiferUtil.h"

USING_NS_MOCKIFER;

using namespace std::chrono;
typedef std::chrono::high_resolution_clock Time;

static MockiferServer *sServer = nullptr;

string MockiferServer::sContentPath = "";
string MockiferServer::sCommandUrl = "";
uint MockiferServer::sCommandPort = 0;
uint MockiferServer::sPort = 0;
uint MockiferServer::sGlobalResponseDelay = 0;

static string LINE_BREAK = "\r\n";
static string COMMAND_URI_PUSH_MOCK = "/mockifer/command/pushmock";
static string COMMAND_URI_CLEAR_MOCKS = "/mockifer/command/clearmocks";
static string COMMAND_URI_RESET = "/mockifer/command/reset";
static string COMMAND_URI_GLOBAL_DELAY = "/mockifer/command/globaldelay";
static string JSON_MOCK_ROUTE_ID = "mockRouteId";
static string JSON_MOCK_NUM_TIMES = "numTimes";
static string JSON_GLOBAL_DELAY = "globalDelay";

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
    fileName = MockiferServer::sContentPath + "/" + fileName;
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
    sContentPath = MockiferUtil::absolutePath(contentPath);
    sPort = stoi(port);
    this->routesManager = unique_ptr<MockiferRouteManager>(new MockiferRouteManager(contentPath));
    serverOk = false;
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
    LOGD("Setting global response delay in milliseconds to %d", delayInMillis);
    MockiferServer::sGlobalResponseDelay = delayInMillis;

    if (sServer && sServer->shouldSendCommandsAsRequests()) {
        ostringstream body;
        body << "{\"" << JSON_GLOBAL_DELAY.c_str() << "\":" << delayInMillis << "}";
        sServer->sendMockiferCommand("POST", COMMAND_URI_GLOBAL_DELAY, body.str());
    }
}

void MockiferServer::sendMockiferCommand(const string &method, const string &uri, const string &body) {
    char postbuf[4 * 1024];
    char ebuf[1000];

    auto contentLength = body.length();

    ostringstream requestFormat;
    requestFormat << method << " " << uri << " ";
    requestFormat << "HTTP/1.1" << LINE_BREAK;
    requestFormat << "Content-Type: application/json" << LINE_BREAK;
    requestFormat << "Content-Length: " << contentLength << LINE_BREAK << LINE_BREAK;

    if (contentLength > 0) {
        requestFormat << body;
    }

    auto format = requestFormat.str();

    struct mg_connection *conn = mg_download(MockiferServer::sCommandUrl.c_str(),
                                             MockiferServer::sCommandPort,
                                             0,
                                             ebuf,
                                             sizeof(ebuf),
                                             "%s", requestFormat.str().c_str());

    if (conn == NULL) {
        return;
    }

    auto timeoutStart = Time::now();

    while (1) {
        int postlen = mg_read(conn, postbuf, sizeof(postbuf) - 1);
        if (postlen <= 0) {
            break;
        }

        // We will time out an internal request after 10 seconds only.
        auto elapsed = duration_cast<milliseconds>(Time::now() - timeoutStart);
        if (elapsed.count() > 10000L) {
            break;
        }
    }

    mg_close_connection(conn);
    fflush(stdout);
}

uint MockiferServer::startOnDynamicPort(const string &contentPath) {
    if (sServer) {
        return sServer->sPort;
    }
    
    sServer = new MockiferServer(contentPath, "0");
    sServer->startServer();
    return sPort;
}

void MockiferServer::start(const string &contentPath, const string &port) {
    if (sServer) {
        return;
    }

    sServer = new MockiferServer(contentPath, port);
    sServer->startServer();
}

void MockiferServer::setCommandUrl(const string &commandUrl, const int &commandPort) {
    LOGD("SET COMMAND URL %s, %d", commandUrl.c_str(), commandPort);

    // We want a basic 'whitelist' of urls that mockifer will be permitted to send commands to.
    // Note that 10.0.2.2 is a special Android url for accessing a host machien localhost.
    if (commandUrl.find("localhost") == string::npos && commandUrl.find("10.0.2.2") == string::npos) {
        LOGD("COMMAND URL REJECTED");
        return;
    }

    MockiferServer::sCommandUrl = commandUrl;
    MockiferServer::sCommandPort = commandPort;
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
    if (sServer) {
        if (sServer->shouldSendCommandsAsRequests()) {
            sServer->sendMockiferCommand("POST", COMMAND_URI_RESET, "{}");
        } else {
            sServer->resetServer();
        }
    }
}

void MockiferServer::resetServer() {
    routesManager->reload();
    duk_get_global_string(jsContext, "mockifer_reset");
    duk_call(jsContext, 0);
    LOGD("Reset server");
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
    auto manifest = MockiferUtil::loadManifest(sContentPath);

    // Load the list of javascript files that need to bootstrap to start the server.
    if (manifest.javascriptBootstrapFiles.size() == 0) {
        serverOk = false;
    }

    for (const auto &javascriptFileName : manifest.javascriptBootstrapFiles) {
        if (!loadJavascriptFile(javascriptFileName)) {
            serverOk = false;
        }
    }

    // Load and register any binary file type suffixes to resolve for requests.
    for (const auto &fileType : manifest.binaryResponseFileTypes) {
        binaryResponseFileTypes.insert(fileType);
    }

    memset(&serverCallbacks, 0, sizeof(serverCallbacks));
    serverCallbacks.begin_request = handleRequest;

    // Note: CivitWeb server is deliberately started with a limit of 1 worker thread
    // so we can achieve a constant ordering of processing the request/response queue.
    auto listenOnBase = "127.0.0.1:";
    const char *options[] = {
        "listening_ports", "",
        "num_threads", "1",
        NULL
    };
    
    if (sPort != 0) {
        auto portString = to_string(sPort);
        LOGD("Starting HTTP server on port: %s", portString.c_str());
        
        auto listenOn = listenOnBase + portString;
        options[1] = listenOn.c_str();
        serverContext = mg_start(&serverCallbacks, NULL, options);
    } else {
        vector<uint> portRange;
        
        // Seed our port range
        for (uint port = 8600; port < 9000; port++) {
            portRange.push_back(port);
        }
        
        // Shuffle them so its unlikely (but still possible) that a collision
        // could occur with multiple servers running within the same port range.
        shuffle(portRange.begin(), portRange.end(), default_random_engine(unsigned(Time::now().time_since_epoch().count())));
        
        // Iterate the port range attempting to start listening on each given port
        // until an available port is found or no available ports are found.
        for (const auto &port : portRange) {
            auto portString = to_string(port);
            LOGD("Attempting to start HTTP server on port: %s", portString.c_str());

            auto listenOn = listenOnBase + portString;
            options[1] = listenOn.c_str();
            serverContext = mg_start(&serverCallbacks, NULL, options);
            
            if (serverContext == NULL) {
                LOGD("Failed to start HTTP server on port: %s", portString.c_str());
            } else {
                sPort = port;
                break;
            }
        }
    }
    
    if (serverContext == NULL) {
        LOGE("FATAL: Unable to start server listening! Check that nothing else is already using the specified port.");
    } else {
        LOGD("Successfully started HTTP server on port: %s", to_string(sPort).c_str());
    }
}

string MockiferServer::getBaseUrl() {
    if (!sServer) {
        return "";
    }

    return "http://localhost:" + to_string(sPort);
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

    auto filePath = sContentPath + "/" + fileName;
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
    MockiferRequest request{requestInfo, requestBody};

    // Only log for non internal Mockifer requests to avoid output noise
    bool shouldLog = request.requestUri.find("/mockifer") == string::npos;

    if (shouldLog) {
        LOGD("Request: [%s] %s", request.requestMethod.c_str(), request.requestUri.c_str());
    }

    // Check if this request is asking for a file whose extension is known to us.
    auto uri = MockiferUtil::toLowerCase(request.requestUri);
    auto fileSuffixIndex = uri.find_last_of(".");
    if (fileSuffixIndex != string::npos) {
        auto fileSuffix = uri.substr(fileSuffixIndex, uri.length());

        // See if its a suffix we recognise as a 'file' response.
        if (sServer->isFileTypeRegistered(fileSuffix)) {
            auto filePath = sContentPath + requestInfo->local_uri;
            mg_send_file(connection, filePath.c_str());

            auto elapsed = duration_cast<milliseconds>(Time::now() - timeBefore);

            if (shouldLog) {
                LOGD(" ↳ Responded in %ld ms with binary file %s", (long) elapsed.count(), requestInfo->local_uri);
            }

            return 1;
        }
    }

    auto response = processRequest(request, shouldLog);

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
    for (auto const &header : response.responseHeaders) {
        responseBody << header.first << ": " << header.second << LINE_BREAK;
    }

    // Response body
    responseBody << LINE_BREAK << body;

    mg_printf(connection, "%s", responseBody.str().c_str());

    auto elapsed = duration_cast<milliseconds>(Time::now() - timeBefore);
    if (shouldLog) {
        LOGD(" ↳ Responded in %ld ms with statusCode %d", (long) elapsed.count(), response.statusCode);
    }

    return 1; // returning 1 indicates we have sent everything to the client.
}

MockiferResponse MockiferServer::processMockiferCommand(MockiferRequest request) {
    MockiferResponse response;
    bool handledSuccessfully = false;

    if (request.requestUri == COMMAND_URI_PUSH_MOCK) {
        Json::Value json;
        Json::Reader reader;

        if (reader.parse(request.requestBody, json)
            && json.isMember(JSON_MOCK_ROUTE_ID)
            && json[JSON_MOCK_ROUTE_ID].isString()
            && json.isMember(JSON_MOCK_NUM_TIMES)
            && json[JSON_MOCK_NUM_TIMES].isUInt()) {

            auto mockRouteId = json[JSON_MOCK_ROUTE_ID].asString();
            auto numTimes = json[JSON_MOCK_NUM_TIMES].asUInt();

            routesManager->pushMock(mockRouteId, numTimes);
        }

        handledSuccessfully = true;
    } else if (request.requestUri == COMMAND_URI_GLOBAL_DELAY) {
        Json::Value json;
        Json::Reader reader;

        if (reader.parse(request.requestBody, json)
            && json.isMember(JSON_GLOBAL_DELAY)
            && json[JSON_GLOBAL_DELAY].isUInt()) {

            MockiferServer::sGlobalResponseDelay = json[JSON_GLOBAL_DELAY].asUInt();
        }

        handledSuccessfully = true;
    } else if (request.requestUri == COMMAND_URI_CLEAR_MOCKS) {
        routesManager->clearActiveMocks();
        handledSuccessfully = true;
    } else if (request.requestUri == COMMAND_URI_RESET) {
        resetServer();
        handledSuccessfully = true;
    }

    if (handledSuccessfully) {
        response.statusCode = 200;
        response.responseBody = request.requestBody;
        response.responseBodyLength = response.responseBody.length();
    }

    // Nope who are you?
    return response;
}

MockiferResponse MockiferServer::processRequest(MockiferRequest request, bool logRequest) {
    auto context = sServer->jsContext;

    // Special commands for internal use only - skips regular request handling.
    if (request.requestUri.find("/mockifer/command/") != string::npos) {
        return sServer->processMockiferCommand(request);
    }

    // Attempt to find a matching route that can handle the request
    auto route = sServer->routesManager->findMatchingRoute(request, logRequest);

    auto requestDto = request.createDto();

    duk_get_global_string(context, "mockifer_handleServerRequest");
    duk_push_string(context, requestDto.c_str());
    duk_push_string(context, route.isValid() ? route.toJson().c_str() : NULL);

    MockiferResponse response;

    bool javascriptCallSuccess = true;

    if (duk_pcall(context, 2) != 0) {
        javascriptCallSuccess = false;
    }

    string javascriptReply = duk_safe_to_string(context, -1);
    if (javascriptCallSuccess) {
        response = MockiferResponse{javascriptReply};
    } else {
        LOGE("Handle server request exception caught for: %s, detail: %s", request.requestUri.c_str(), javascriptReply.c_str());
        response.responseBody = "{\"mockiferJavascriptEngineError\": \"" + javascriptReply + "\"}";
        response.responseBodyLength = response.responseBody.length();
    }

    duk_pop(context);

    // Simulate a response delay based on whether the current route had a custom delay ...
    auto responseDelay = route.responseDelay;
    if (responseDelay == 0) {
        // or if not, adopt the global response delay of the server.
        responseDelay = MockiferServer::sGlobalResponseDelay;
    }

    // If there is a delay to respect, simulate it. This works because requests are handled
    // on a single worker thread so it will just make the next request wait its turn.
    if (responseDelay > 0) {
        this_thread::sleep_for(chrono::milliseconds(responseDelay));
    }

    return response;
}

#pragma mark Routes manager interaction

bool MockiferServer::shouldSendCommandsAsRequests() {
    return MockiferServer::sCommandUrl.length() > 0 && MockiferServer::sCommandPort > 0;
}

bool MockiferServer::pushMock(const string &mockRouteId, const int &times) {
    if (sServer) {
        if (sServer->shouldSendCommandsAsRequests()) {
            ostringstream body;
            body << "{\"" << JSON_MOCK_ROUTE_ID.c_str() << "\":\"" << mockRouteId << "\", \"numTimes\":" << times << "}";
            sServer->sendMockiferCommand("POST", COMMAND_URI_PUSH_MOCK, body.str());
        } else {
            return sServer->routesManager->pushMock(mockRouteId, times);
        }
    }

    return false;
}

bool MockiferServer::clearActiveMocks() {
    if (sServer) {
        if (sServer->shouldSendCommandsAsRequests()) {
            sServer->sendMockiferCommand("POST", COMMAND_URI_CLEAR_MOCKS, "{}");
        } else {
            sServer->routesManager->clearActiveMocks();
        }

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
