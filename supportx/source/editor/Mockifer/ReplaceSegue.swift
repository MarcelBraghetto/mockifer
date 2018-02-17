import Foundation
import Cocoa

class ReplaceSegue: NSStoryboardSegue {
    override func perform() {
        let source = self.sourceController as? NSViewController
        let destination = self.destinationController as? NSViewController
        
        if (source == nil) {
            let window = NSApplication.shared.keyWindow
            window?.contentViewController = destination
            window?.contentViewController?.removeFromParentViewController()
        } else {
            source?.view.window?.contentViewController = destination
            source?.removeFromParentViewController()
        }
    }
}
