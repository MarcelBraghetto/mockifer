import Foundation

class ServerConfig {
    static let serverUrl = "http://localhost"
    
    // Note: The normal app can be pointed at any server / port but the most common scenarios are to point
    // at either the Mockifer Editor, or the embedded automation suite. The embedded automation suite
    // would typically be used when running automation tests on a CI server or the like. To switch between
    // these scenarios, comment/uncomment the appropriate server port below before running the app or UI tests.
    
    // Uncomment to run the normal app, and any automation tests against the Mockifer Editor on port 8501.
    static let serverPort: uint = 8501

    // Uncomment to run the normal app, and any automation tests against the Mockifer Console application on port 8504.
    //static let serverPort: uint = 8504

    // Uncomment to run the normal app against the embedded automation Mockifer instance.
    //static let serverPort: uint = 8502
}
