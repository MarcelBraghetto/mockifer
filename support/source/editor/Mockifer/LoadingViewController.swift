import Foundation
import Cocoa

class LoadingViewController : BaseViewController {
    @IBOutlet var LoadingTextView: NSTextView!
    
    private let tag = " ↳ "
    private let lineBreak = "\r\n"
    private let keyLine = "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
    private let appFolder = Bundle.main.bundleURL.deletingLastPathComponent().path
    
    func appendLoadingTextLine(text: String) {
        DispatchQueue.main.async {
            self.LoadingTextView.string.append(text)
        }
    }
    
    func refreshContent() -> Bool {
        var needsContentRefresh = true
        
        self.appendLoadingTextLine(text: "\(self.tag) Checking for compiled content.\(self.lineBreak)")
        do {
            // We will attempt to fetch a known compiled content file and access its file attributes.
            let attributes = try FileManager.default.attributesOfItem(atPath: "\(self.appFolder)/build/content/editor/mockifer-js/mockifer.properties")
            
            self.appendLoadingTextLine(text: "\(self.tag) Checking age of local content.\(self.lineBreak)")
            
            if let fileCreationDate = attributes[FileAttributeKey.creationDate] as? NSDate {
                // Looking at the file creation date, decide if it is 'stale', indicating that it would be a good idea to recompile the project to make sure we aren't using an out of date content source.
                let maxContentAge = 1000 * 60 * 5; // 5 minutes
                let now = Int(Date().timeIntervalSince1970 * 1000);
                let fileCreationTime = Int(fileCreationDate.timeIntervalSince1970 * 1000);
                let age = now - fileCreationTime
                
                // If the content file was created within an acceptable window of time, we can just use the content as-is.
                if (age < maxContentAge) {
                    needsContentRefresh = false
                }
            }
        } catch {
            self.appendLoadingTextLine(text: "\(self.tag) Compiled content not found.\(self.lineBreak)")
        }
        
        if (needsContentRefresh) {
            self.appendLoadingTextLine(text: "\(self.tag) Content is missing or stale - recompile needed.\(self.lineBreak)")
        } else {
            self.appendLoadingTextLine(text: "\(self.tag) Content is ready to use.\(self.lineBreak)")
            return true
        }
        
        var compilationSuccessful = false
        
        if (FileManager.default.fileExists(atPath: "\(self.appFolder)/build_content.sh")) {
            self.appendLoadingTextLine(text: "\(self.tag) Invoking compilation script.\(self.lineBreak)\(self.keyLine)\(self.lineBreak)")
            
            // Kick off the compilation script to cause the content to be recompiled fresh.
            let task = Process()
            task.launchPath = "\(self.appFolder)/build_content.sh"
            
            let pipe = Pipe()
            task.standardOutput = pipe
            task.standardError = pipe
            
            let outHandle = pipe.fileHandleForReading
            outHandle.readabilityHandler = { pipe in
                if let line = String(data: pipe.availableData, encoding: String.Encoding.utf8) {
                    self.appendLoadingTextLine(text: line)
                }
            }
            
            task.launch()
            task.waitUntilExit()
            
            if (task.terminationStatus == 0) {
                compilationSuccessful = true
            }
        } else {
            self.appendLoadingTextLine(text: "\(self.keyLine)\(self.lineBreak)✪ WARNING: Compile script not found!\(self.lineBreak)\(self.keyLine)\(self.lineBreak)")
        }
        
        if (compilationSuccessful) {
            self.appendLoadingTextLine(text: "\(self.keyLine)\(self.lineBreak)✪ Compilation script complete. \(self.lineBreak)\(self.keyLine)\(self.lineBreak)")
            
            return true
        } else {
            self.appendLoadingTextLine(text: "\(self.keyLine)\(self.lineBreak)✪ WARNING: Content compilation failed!\(self.lineBreak)\(self.keyLine)\(self.lineBreak)")
            
            return false
        }
    }
    
    override func viewDidAppear() {
        super.viewDidAppear()
        
        LoadingTextView.string = "\(keyLine)\(self.lineBreak)✪ Welcome to Mockifer - preparing to start.\(lineBreak)\(keyLine)\(lineBreak)"
        
        DispatchQueue.global(qos: .userInitiated).async {
            if (self.refreshContent()) {

                DispatchQueue.main.asyncAfter(deadline: .now() + .milliseconds(1000)) {
                    self.appendLoadingTextLine(text: "\(self.tag) Booting Mockifer engine, please hold ...\(self.lineBreak)")
                    
                    Mockifer.start()
                    
                    self.appendLoadingTextLine(text: "\(self.tag) Mockifer engine booted, navigating to editor.\(self.lineBreak)")

                    self.performSegue(withIdentifier: "NavigateToMain", sender: nil)
                }
            }
        }
    }
}

