import Cocoa

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {
    override init() {
        Mockifer.start()
    }
    
    func applicationDidFinishLaunching(_ notification: Notification) {
    }
    
    func applicationWillTerminate(_ notification: Notification) {
        Mockifer.stop()
    }
}
