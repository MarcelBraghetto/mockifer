import UIKit
import Mockifer

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {
    var window: UIWindow?
    
    override init() {
        Mockifer.start()
    }
    
    deinit {
        Mockifer.stop()
    }
}

