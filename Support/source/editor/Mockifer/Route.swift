import Foundation

class Route : NSObject {
    let ROUTE_ID = "routeId"
    let ROUTE_IS_INTERNAL = "isInternal"
    let ROUTE_DISPLAY_NAME = "routeDisplayName"
    let ROUTE_DESCRIPTION = "routeDescription"
    let REQUEST_METHOD = "requestMethod"
    let REQUEST_URI = "requestUri"
    let REQUEST_OVERRIDE_ROUTE_ID = "requestOverrideRouteId"
    let REQUEST_QUERY_STRING_CONTAINS = "requestQueryStringContains"
    let REQUEST_QUERY_STRING_EQUALS = "requestQueryStringEquals"
    let REQUEST_BODY_JSON_PATH = "requestBodyJsonPath"
    let REQUEST_BODY_JSON_PATH_CONTAINS = "requestBodyJsonPathContains"
    let REQUEST_BODY_JSON_PATH_EQUALS = "requestBodyJsonPathEquals"
    let RESPONSE_STATUS_CODE = "responseStatusCode"
    let RESPONSE_DELAY = "responseDelay"
    let RESPONSE_JSON_FILE = "responseJsonFile"
    let RESPONSE_CONTROLLER_ID = "responseControllerId"
    
    @objc dynamic var responseStatusCode = UInt(0)
    @objc dynamic var responseDelay = UInt(0)
    @objc dynamic var routeId = ""
    @objc dynamic var isInternal = false
    @objc dynamic var routeDisplayName = ""
    @objc dynamic var routeDescription = ""
    @objc dynamic var requestMethod = ""
    @objc dynamic var requestUri = ""
    @objc dynamic var requestOverrideRouteId = ""
    @objc dynamic var requestQueryStringContains = ""
    @objc dynamic var requestQueryStringEquals = ""
    @objc dynamic var requestBodyJsonPath = ""
    @objc dynamic var requestBodyJsonPathContains = ""
    @objc dynamic var requestBodyJsonPathEquals = ""
    @objc dynamic var responseJsonFile = ""
    @objc dynamic var responseControllerId = ""
    
    let searchableText: String
    
    func copyOf() -> Route {
        let route = Route()
        route.routeId = routeId
        route.isInternal = isInternal
        route.routeDisplayName = routeDisplayName
        route.routeDescription = routeDescription
        route.requestMethod = requestMethod
        route.requestUri = requestUri
        route.requestOverrideRouteId = requestOverrideRouteId
        route.requestQueryStringContains = requestQueryStringContains
        route.requestQueryStringEquals = requestQueryStringEquals
        route.requestBodyJsonPath = requestBodyJsonPath
        route.requestBodyJsonPathContains = requestBodyJsonPathContains
        route.requestBodyJsonPathEquals = requestBodyJsonPathEquals
        route.responseStatusCode = responseStatusCode
        route.responseDelay = responseDelay
        route.responseJsonFile = responseJsonFile
        route.responseControllerId = responseControllerId
        return route
    }
    
    init(source: Dictionary<String, AnyObject>? = nil) {
        
        if let source = source {
            if let routeId = source[ROUTE_ID] as? String {
                self.routeId = routeId
            }
            
            if let isInternal = source[ROUTE_IS_INTERNAL] as? Bool {
                self.isInternal = isInternal
            }
            
            if let routeDisplayName = source[ROUTE_DISPLAY_NAME] as? String {
                self.routeDisplayName = routeDisplayName
            }
            
            if let routeDescription = source[ROUTE_DESCRIPTION] as? String {
                self.routeDescription = routeDescription
            }
            
            if let requestMethod = source[REQUEST_METHOD] as? String {
                self.requestMethod = requestMethod
            }
            
            if let requestUri = source[REQUEST_URI] as? String {
                self.requestUri = requestUri
            }
            
            if let requestOverrideRouteId = source[REQUEST_OVERRIDE_ROUTE_ID] as? String {
                self.requestOverrideRouteId = requestOverrideRouteId
            }
            
            if let requestQueryStringContains = source[REQUEST_QUERY_STRING_CONTAINS] as? String {
                self.requestQueryStringContains = requestQueryStringContains
            }
            
            if let requestQueryStringEquals = source[REQUEST_QUERY_STRING_EQUALS] as? String {
                self.requestQueryStringEquals = requestQueryStringEquals
            }
            
            if let requestBodyJsonPath = source[REQUEST_BODY_JSON_PATH] as? String {
                self.requestBodyJsonPath = requestBodyJsonPath
            }
            
            if let requestBodyJsonPathContains = source[REQUEST_BODY_JSON_PATH_CONTAINS] as? String {
                self.requestBodyJsonPathContains = requestBodyJsonPathContains
            }
            
            if let requestBodyJsonPathEquals = source[REQUEST_BODY_JSON_PATH_EQUALS] as? String {
                self.requestBodyJsonPathEquals = requestBodyJsonPathEquals
            }
            
            if let responseStatusCode = source[RESPONSE_STATUS_CODE] as? UInt {
                self.responseStatusCode = responseStatusCode
            }

            if let responseDelay = source[RESPONSE_DELAY] as? UInt {
                self.responseDelay = responseDelay
            }

            if let responseJsonFile = source[RESPONSE_JSON_FILE] as? String {
                self.responseJsonFile = responseJsonFile
            }
            
            if let responseControllerId = source[RESPONSE_CONTROLLER_ID] as? String {
                self.responseControllerId = responseControllerId
            }
        }
        
        searchableText = "\(routeId)\(routeDisplayName)\(routeDescription)\(requestMethod)\(requestUri)\(responseControllerId)\(responseJsonFile)".uppercased()
        
        super.init()
    }
    
    func toJson() -> String {
        
        let data = NSMutableDictionary()
        
        data[ROUTE_ID] = routeId
        data[ROUTE_IS_INTERNAL] = isInternal
        data[ROUTE_DISPLAY_NAME] = routeDisplayName
        data[ROUTE_DESCRIPTION] = routeDescription
        data[REQUEST_METHOD] = requestMethod
        data[REQUEST_URI] = requestUri
        data[REQUEST_OVERRIDE_ROUTE_ID] = requestOverrideRouteId
        data[REQUEST_QUERY_STRING_CONTAINS] = requestQueryStringContains
        data[REQUEST_QUERY_STRING_EQUALS] = requestQueryStringEquals
        data[REQUEST_BODY_JSON_PATH] = requestBodyJsonPath
        data[REQUEST_BODY_JSON_PATH_CONTAINS] = requestBodyJsonPathContains
        data[REQUEST_BODY_JSON_PATH_EQUALS] = requestBodyJsonPathEquals
        data[RESPONSE_STATUS_CODE] = responseStatusCode
        data[RESPONSE_DELAY] = responseDelay
        data[RESPONSE_JSON_FILE] = responseJsonFile
        data[RESPONSE_CONTROLLER_ID] = responseControllerId
        
        var json = ""
        
        do {
            let jsonData = try JSONSerialization.data(withJSONObject: data, options: .prettyPrinted)
            json = String(data: jsonData, encoding: .utf8)!
        } catch {
            // Handle Error
        }
        
        return json
    }
}
