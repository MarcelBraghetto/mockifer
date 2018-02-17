import UIKit
import Mockifer

// Important: Mockifer is embedded into the 'mock' target only.

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {
    var window: UIWindow?
    
    override init() {
        Mockifer.start(onPort: "\(ServerConfig.serverPort)")
    }
    
    deinit {
        Mockifer.stop()
    }
}
