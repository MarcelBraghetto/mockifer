import Cocoa

class BaseViewController: NSViewController, NSWindowDelegate {
    
    override func viewDidAppear() {
        self.view.window?.delegate = self
    }
    
    override func viewDidDisappear() {
        self.view.window?.delegate = nil
    }
    
    func windowShouldClose(_ sender: NSWindow) -> Bool {
        NSApplication.shared.terminate(self)
        return true
    }
}
