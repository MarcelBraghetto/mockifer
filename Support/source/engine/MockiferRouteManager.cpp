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

#include "MockiferRouteManager.h"
#include "MockiferUtil.h"
#include <fstream>

USING_NS_MOCKIFER;

static const char* ROUTES = "routes";
static const char* ACTIVE_MOCKS = "activeMocks";
static const char* ROUTES_FILE_PATH = "/routes.json";
static const char* MOCKS_FILE_PATH = "/data/mocks/";
static const char* MOCK_FILE_SUFFIX = ".json";

MockiferRouteManager::MockiferRouteManager(const string &sourceContentPath) {
    this->sourceContentPath = sourceContentPath;
    addDataSyncPath(sourceContentPath);
}

bool MockiferRouteManager::reload() {
    auto loadRoutesResult = MockiferUtil::loadTextFile(sourceContentPath + ROUTES_FILE_PATH);
    
    if (loadRoutesResult.first == false) {
        LOGE("ERROR: MockiferRouteManager::reload: Unable to load routes.json file.");
        return false;
    }
    
    Json::Value root;
    Json::Reader reader;
    
    bool parsingSuccessful = reader.parse(loadRoutesResult.second, root);
    if (!parsingSuccessful) {
        LOGE("ERROR: MockiferRouteManager::reload: Unable to parse routes.json file.");
        return false;
    }

    if (!root.isMember(ROUTES) || !root[ROUTES].isArray()) {
        LOGE("ERROR: MockiferRouteManager::reload: Routes file routes.json has no 'routes' element.");
        return false;
    }
    
    allRoutes.clear();
    internalRoutes.clear();
    activeMockRoutes.clear();
    
    const auto &routes = root[ROUTES];
    
    for (int i = 0; i < routes.size(); ++i) {
        auto route = MockiferRoute();
        
        if (route.populateFromJsonObject(routes[i])) {
            allRoutes.push_back(route);
            
            if (route.isInternal) {
                internalRoutes.push_back(route);
            }
        }
    }
    
    // Sort the internal routes in order of routes with the most filters to least so
    // the strictest route matching rules run before the more relaxed ones.
    sort(internalRoutes.begin(),
         internalRoutes.end(),
         [] (MockiferRoute& lhs, MockiferRoute& rhs) {
             return lhs.getFilterCount() < rhs.getFilterCount();
         });
    
    return true;
}

void MockiferRouteManager::addDataSyncPath(const string &syncPath) {
    auto path = MockiferUtil::absolutePath(syncPath);
    auto pathLength = path.length();
    if (pathLength > 0 && path[pathLength - 1] == '/') {
        path.erase(pathLength - 1);
    }
    
    if (!MockiferUtil::pathExists(path, true)) {
        LOGE("MockiferRouteManager::addDataSyncPath: couldn't find path: %s", path.c_str());
        return;
    }
    
    if (!MockiferUtil::pathExists(path + ROUTES_FILE_PATH, false)) {
        LOGE("MockiferRouteManager::addDataSyncPath: couldn't find 'routes.json' in path.");
        return;
    }
    
    LOGD("MockiferRouteManager::addDataSyncPath: added path: %s", path.c_str());
    
    dataSyncPaths.push_back(path);
}

void MockiferRouteManager::clearActiveMocks() {
    activeMockRoutes.clear();
    LOGD("Cleared active mocks");
}

string MockiferRouteManager::getAllRoutesAsJson() {
    Json::StyledWriter writer;
    Json::Value root;
    
    Json::Value routesArray(Json::arrayValue);
    
    for (auto route : allRoutes) {
        routesArray.append(route.toJsonObject());
    }
    
    root[ROUTES] = routesArray;
    
    return writer.write(root);
}

string MockiferRouteManager::getAllActiveMocksAsJson() {
    Json::StyledWriter writer;
    Json::Value root;
    
    Json::Value activeMocksArray(Json::arrayValue);
    
    for (auto activeMock : activeMockRoutes) {
        activeMocksArray.append(activeMock.toJsonObject());
    }
    
    root[ACTIVE_MOCKS] = activeMocksArray;
    
    return writer.write(root);
}

bool MockiferRouteManager::pushMock(const string &mockRouteId, const int &times) {
    for (const auto &route : allRoutes) {
        if (route.routeId == mockRouteId) {
            for (int i = 0; i < times; i++) {
                activeMockRoutes.push_back(route);
            }
            LOGD("*** Pushed mock route '%s' '%d' times.", mockRouteId.c_str(), times);
            return true;
        }
    }
    
    LOGD("Attempted to add an unknown mock route '%s'. Use the editor and make sure the mock route is registered in 'routes.json' before using it.", mockRouteId.c_str());
    return false;
}

bool MockiferRouteManager::updateExistingRoute(const string &routeJson, const string &jsonFileContent) {
    auto route = MockiferRoute();
    
    if (!route.populateFromJsonString(routeJson)) {
        LOGE("ERROR: updateExistingRoute: Could not parse route json for updating.");
        return false;
    }
    
    if (!route.isValid()) {
        LOGE("ERROR: updateExistingRoute: route was not valid - did not update.");
        return false;
    }
    
    for (int i = 0; i < allRoutes.size(); i++) {
        if (route.routeId == allRoutes[i].routeId) {
            allRoutes[i] = route;
            
            if (jsonFileContent.empty()) {
                route.responseJsonFile = "";
                deleteMockDataFile(route.routeId);
            } else {
                route.responseJsonFile = route.routeId + MOCK_FILE_SUFFIX;
                saveMockDataFile(route.routeId, jsonFileContent);
            }

            return saveAndReload();
        }
    }

    LOGE("ERROR: updateExistingRoute: route to update was not found");
    return false;
}

bool MockiferRouteManager::saveAndReload() {
    auto success = true;
    auto routesJson = getAllRoutesAsJson();
    
    for (const auto &syncPath : dataSyncPaths) {
        if (!MockiferUtil::saveTextFile(syncPath + ROUTES_FILE_PATH, routesJson)) {
            success = false;
        }
    }
    
    return success;
}

bool MockiferRouteManager::createNewRoute(const string &routeJson, const string &jsonFileContent) {
    auto newRoute = MockiferRoute();
    newRoute.populateFromJsonString(routeJson);
    
    if (!newRoute.isValid()) {
        LOGE("ERROR: MockiferRouteManager::createNewRoute: invalid route json.");
        return false;
    }
    
    for (const auto &route : allRoutes) {
        if (route.routeId == newRoute.routeId) {
            LOGE("ERROR: MockiferRouteManager::createNewRoute: route id '%s' already exists, route id's must be unique.", newRoute.routeId.c_str());
            return false;
        }
    }
    
    allRoutes.push_back(newRoute);
    
    if (!jsonFileContent.empty()) {
        saveMockDataFile(newRoute.routeId, jsonFileContent);
    }
    
    return saveAndReload();
}

bool MockiferRouteManager::deleteRoute(const string &routeId) {
    for (int i = 0; i < allRoutes.size(); i++) {
        if (routeId == allRoutes[i].routeId) {
            allRoutes.erase(allRoutes.begin() + i);
            deleteMockDataFile(routeId);
            return saveAndReload();
        }
    }
    
    return false;
}

string MockiferRouteManager::loadMockDataFile(const string &routeId) {
    auto loadResult = MockiferUtil::loadTextFile(sourceContentPath + MOCKS_FILE_PATH + routeId + MOCK_FILE_SUFFIX);
    
    if (loadResult.first == false) {
        LOGE("ERROR: MockiferRouteManager::loadMockDataFile: Failed to load data file for route id '%s'", routeId.c_str());
        return "";
    }
    
    return loadResult.second;
}

bool MockiferRouteManager::deleteMockDataFile(const string &routeId) {
    auto success = true;
    
    for (const auto &syncPath : dataSyncPaths) {
        auto filePath = syncPath + MOCKS_FILE_PATH + routeId + MOCK_FILE_SUFFIX;
        if (!MockiferUtil::deleteFile(syncPath + MOCKS_FILE_PATH + routeId + MOCK_FILE_SUFFIX)) {
            success = false;
        }
    }
    
    return success;
}

bool MockiferRouteManager::saveMockDataFile(const string &routeId, const string &content) {
    auto success = true;
    
    for (const auto &syncPath : dataSyncPaths) {
        if (!MockiferUtil::saveTextFile(syncPath + MOCKS_FILE_PATH + routeId + MOCK_FILE_SUFFIX, content)) {
            success = false;
        }
    }
    
    return success;
}

MockiferRoute MockiferRouteManager::findMatchingRoute(const MockiferRequest &request, bool logRequest) {
    // To begin with, attempt to match and consume any currently active mock routes.
    auto result = consumeActiveMock(request);
    if (result.isValid()) {
        
        if (logRequest) {
            LOGD("Mock match: %s", result.routeId.c_str());
        }
        return result;
    }

    auto exactMatchFound = false;

    // Then attempt to match against any 'internal' routes with an exact match uri comparison.
    for (const auto &internalRoute : internalRoutes) {
        if (isMatch(internalRoute, request, true)) {
            // If we found a matching 'internal' route, we need to do one more pass to see
            // if there are any 'active mock' routes that want to hijack the matched route id
            // and take over instead.
            auto activeMockRoute = consumeActiveMock(internalRoute);
            
            if (activeMockRoute.isValid()) {
                if (logRequest) {
                    LOGD("Mock (%s) intercepted internal route (%s)", activeMockRoute.routeId.c_str(), internalRoute.routeId.c_str());
                }
                result = activeMockRoute;
            } else {
                if (logRequest) {
                    LOGD("Internal route match: %s", internalRoute.routeId.c_str());
                }
                result = internalRoute;
            }

            exactMatchFound = true;
            break;
        }
    }

    if (!exactMatchFound) {
        // Then attempt to match against any 'internal' routes with a request uri regex comparison.
        for (const auto &internalRoute : internalRoutes) {
            if (isMatch(internalRoute, request, false)) {
                // If we found a matching 'internal' route, we need to do one more pass to see
                // if there are any 'active mock' routes that want to hijack the matched route id
                // and take over instead.
                auto activeMockRoute = consumeActiveMock(internalRoute);

                if (activeMockRoute.isValid()) {
                    if (logRequest) {
                        LOGD("Mock (%s) intercepted internal route (%s)", activeMockRoute.routeId.c_str(), internalRoute.routeId.c_str());
                    }
                    result = activeMockRoute;
                } else {
                    if (logRequest) {
                        LOGD("Internal route match: %s", internalRoute.routeId.c_str());
                    }
                    result = internalRoute;
                }

                break;
            }
        }
    }

    return result;
}

MockiferRoute MockiferRouteManager::consumeActiveMock(const MockiferRequest &request) {
    MockiferRoute result;
    auto exactMatchFound = false;
    
    // Look through all our registered 'active mock routes' and if we find one that
    // matches the given request with an EXACT uri, 'consume' it and return the result.
    for (int i = 0; i < activeMockRoutes.size(); i++) {
        const auto &mockRoute = activeMockRoutes[i];
        if (isMatch(mockRoute, request, true)) {
            result = mockRoute;
            activeMockRoutes.erase(activeMockRoutes.begin() + i);
            exactMatchFound = true;
            break;
        }
    }

    if (!exactMatchFound) {
        // Look through all our registered 'active mock routes' and if we find one that
        // matches the given request with a matching regex uri, 'consume' it and return the result.
        for (int i = 0; i < activeMockRoutes.size(); i++) {
            const auto &mockRoute = activeMockRoutes[i];
            if (isMatch(mockRoute, request, false)) {
                result = mockRoute;
                activeMockRoutes.erase(activeMockRoutes.begin() + i);
                break;
            }
        }
    }

    return result;
}

MockiferRoute MockiferRouteManager::consumeActiveMock(const MockiferRoute &route) {
    MockiferRoute result;
    
    // Look through all our registered 'active mock routes routes' and if we find one that
    // has opted in to override another route id, 'consume' it and return the result.
    for (int i = 0; i < activeMockRoutes.size(); i++) {
        const auto &mockRoute = activeMockRoutes[i];
        if (mockRoute.requestOverrideRouteId == route.routeId) {
            result = mockRoute;
            activeMockRoutes.erase(activeMockRoutes.begin() + i);
            break;
        }
    }
    
    return result;
}

bool MockiferRouteManager::isMatch(const MockiferRoute &route, const MockiferRequest &request, const bool &matchUriExactly) {
    
    // Match against the request method (GET/PUT/POST etc)
    if (route.requestMethod != request.requestMethod) {
        return false;
    }

    if (matchUriExactly) {
        // See if the request uri exactly matches the route's uri.
        if (request.requestUri != route.requestUri) {
            return false;
        }
    } else {
        // See if the request uri matches the route's uri regular expression.
        if (!regex_match(request.requestUri, route.uriRegex)) {
            return false;
        }
    }

    // If the route has a 'query string contains' filter applied...
    if (!route.requestQueryStringContains.empty()) {
        // Only proceed if the request query string 'contains' the given filter.
        if (request.requestQueryString.find(route.requestQueryStringContains) == string::npos) {
            return false;
        }
    }
    
    // If the route has a 'query string equals' filter applied...
    if (!route.requestQueryStringEquals.empty()) {
        // Only proceed if the request query string 'equals' the given filter.
        if (request.requestQueryString != route.requestQueryStringEquals) {
            return false;
        }
    }
    
    // There was a json body 'path' listed as a validation criteria
    if (!route.requestBodyJsonPath.empty()) {
        
        // Although there was a json body path, there was no criteria which is invalid...
        if (route.requestBodyJsonPathContains.empty() && route.requestBodyJsonPathEquals.empty()) {
            return false;
        }
        
        // There was a verify against the json path 'containing' a value
        if (!route.requestBodyJsonPathContains.empty()) {
            auto crawlResult = crawlRequestBodyJsonForValue(route.requestBodyJsonPath, request.requestBodyJson);
            if (crawlResult.first == false) {
                return false;
            }
            
            return crawlResult.second.find(route.requestBodyJsonPathContains) != string::npos;
        }
        
        // There was a verify against the json path 'equals' a value
        if (!route.requestBodyJsonPathEquals.empty()) {
            auto crawlResult = crawlRequestBodyJsonForValue(route.requestBodyJsonPath, request.requestBodyJson);
            if (crawlResult.first == false) {
                return false;
            }
            
            return crawlResult.second == route.requestBodyJsonPathContains;
        }
    }
    
    // If we reach this point, then all the filters passed therefore we should assume
    // that the given route 'matches' the given request.
    return true;    
}

pair<bool, string> MockiferRouteManager::crawlRequestBodyJsonForValue(const string &jsonPath, const Json::Value &jsonRoot) {
    auto tokens = MockiferUtil::split(jsonPath, '/');
    
    // No tokens.. can't exactly crawl along nothing...
    if (tokens.size() == 0) {
        return make_pair(false, "");
    }

    // Start at the root node of the request body.
    auto node = jsonRoot;
    
    auto numTokens = tokens.size();
    for (auto i = 0; i < numTokens; i++) {
        auto token = tokens[i];
        
        // See if the token was actually a number, which would indicate that the path should find
        // an element in an array.
        char *indexConversionResult;
        auto arrayIndex = strtol(token.c_str(), &indexConversionResult, 10);
        
        if (*indexConversionResult) {
            // The token was not a number, so we will expect that it is an object.
            if (!node.isObject() || !node.isMember(token)) {
                return make_pair(false, "");
            }
            
            // Move forward to the next node which should be an object.
            node = node[token];
        } else {
            // The token was a number, therefore we are trying to get to an array element.
            if (!node.isArray()) {
                return make_pair(false, "");
            }
            
            // The array index is higher than the number of elements in the json array.
            if (node.size() - 1 < arrayIndex) {
                return make_pair(false, "");
            }
            
            // Move forward to the element in the json array for the array index.
            node = node[(int) arrayIndex];
        }
        
        // We reached the final token, which should be the termination point for the path evaluation.
        if (i == numTokens - 1) {
            // We should be at a leaf node now, if not then the path cannot be evaluated.
            if (node.isArray() || node.isObject() || node.isNull()) {
                return make_pair(false, "");
            }
            
            // Take the value of the leaf node as a string - we found it!
            // Note that all values will be returned as strings regardless of what primitive
            // data type they represent.
            return make_pair(true, node.asString());
        }
    }
    
    // Fallback can't have succeeded in the crawl match.
    return make_pair(false, "");
}
