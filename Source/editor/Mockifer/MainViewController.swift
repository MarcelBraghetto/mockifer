import Foundation
import AppKit

class MainViewController : BaseViewController, NSTableViewDelegate, NSTextViewDelegate, NSTextFieldDelegate {
    @IBOutlet weak var searchField: NSSearchField!
    @IBOutlet weak var progressSpinner: NSProgressIndicator!
    @IBOutlet weak var tableView: NSTableView!
    @IBOutlet var routesController: NSArrayController!
    @IBOutlet var activeMocksController: NSArrayController!
    @IBOutlet weak var infoLabel: NSTextField!

    @IBOutlet weak var pushMockButton: NSButton!
    @IBOutlet weak var routeIdField: NSTextField!
    @IBOutlet weak var isInternalCheckbox: NSButton!
    @IBOutlet weak var routeDisplayNameField: NSTextField!
    @IBOutlet weak var routeDescriptionField: NSTextField!
    @IBOutlet weak var requestMethodField: NSTextField!
    @IBOutlet weak var requestRouteOverrideField: NSTextField!
    @IBOutlet weak var requestQueryStringContainsField: NSTextField!
    @IBOutlet weak var requestQueryStringEqualsField: NSTextField!
    @IBOutlet weak var requestUriField: NSTextField!
    
    @IBOutlet weak var requestBodyJsonPathField: NSTextField!
    @IBOutlet weak var requestBodyJsonPathContainsField: NSTextField!
    @IBOutlet weak var requestBodyJsonPathEqualsField: NSTextField!
    
    @IBOutlet weak var responseDelayField: NSTextField!
    
    @IBOutlet weak var responseStatusCodeField: NSTextField!
    @IBOutlet weak var responseControllerIdField: NSTextField!
    @IBOutlet weak var jsonContentFileField: NSTextField!
    
    @IBOutlet var jsonContentField: NSTextView!
    private var allRoutes = [Route]()
    private var filteredRoutes = [Route]()
    private var activeMockRoutes = [Route]()
    private var currentRoute: Route = Route()
    private var isEditingNewMock = true
    
    private let idFormatter = InputFormatter(permittedCharacters: "abcdefghijklmnopqrstuvwxyz0123456789.-_", withMaxLength: 50)
    private let statusCodeFormatter = InputFormatter(permittedCharacters: "0123456789", withMaxLength: 3)
    private let responseDelayFormatter = InputFormatter(permittedCharacters: "0123456789", withMaxLength: 5)
    
    private var activeMocksRefreshTimer: Timer?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        infoLabel.stringValue = "Mockifer base url: \(Mockifer.getBaseUrl())"
        
        jsonContentField.isRichText = false
        jsonContentField.isContinuousSpellCheckingEnabled = false
        jsonContentField.isAutomaticTextReplacementEnabled = false
        jsonContentField.isGrammarCheckingEnabled = false
        jsonContentField.isAutomaticDataDetectionEnabled = false
        jsonContentField.isAutomaticLinkDetectionEnabled = false
        jsonContentField.isAutomaticDashSubstitutionEnabled = false
        jsonContentField.isAutomaticQuoteSubstitutionEnabled = false
        jsonContentField.isAutomaticSpellingCorrectionEnabled = false
        
        routeIdField.delegate = self
        requestRouteOverrideField.delegate = self
        responseControllerIdField.delegate = self
        responseStatusCodeField.delegate = self
        responseDelayField.delegate = self
        
        reloadRoutes()
        reloadActiveMocks()
        startNewRoute()
    }
    
    override func viewDidAppear() {
        super.viewDidAppear()
        activeMocksRefreshTimer?.invalidate()
        activeMocksRefreshTimer = Timer.scheduledTimer(timeInterval: 5, target: self, selector: #selector(reloadActiveMocks), userInfo: nil, repeats: true)
    }
    
    override func viewDidDisappear() {
        super.viewDidDisappear()
        activeMocksRefreshTimer?.invalidate()
    }
    
    func reloadActiveMocks() {
        Requester.load(method: "GET", uri: "/mockifer/activemocks", body: nil) { (data, error) in
            self.activeMockRoutes = [Route]()
            
            guard let data = data else {
                self.refreshActiveMockRoutes()
                return
            }
            
            do {
                guard let routeData = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as? [String : AnyObject] else {
                    self.refreshActiveMockRoutes()
                    return
                }
                
                if let results = routeData["activeMocks"] as? [[String : AnyObject]] {
                    self.activeMockRoutes = results.map {
                        return Route(source: $0)
                    }
                }
            } catch _ {
                
            }
            
            self.refreshActiveMockRoutes()
        }
    }
    
    func reloadRoutes() {
        Requester.load(method: "GET", uri: "/mockifer/routes", body: nil) { (data, error) in
            self.allRoutes = [Route]()
            
            guard let data = data else {
                self.refreshRoutes()
                return
            }
            
            do {
                guard let routeData = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as? [String : AnyObject] else {
                    self.refreshRoutes()
                    return
                }
                
                if let results = routeData["routes"] as? [[String : AnyObject]] {
                    self.allRoutes = results.map {
                        return Route(source: $0)
                    }
                }
            } catch _ {
                
            }
            
            self.refreshRoutes()
        }
    }
    
    func showLoadingIndicator() {
        progressSpinner.isHidden = false
        progressSpinner.isIndeterminate = true
        progressSpinner.usesThreadedAnimation = true
        progressSpinner.startAnimation(nil)
    }
    
    func hideLoadingIndicator() {
        progressSpinner.isHidden = true
        progressSpinner.stopAnimation(nil)
    }
    
    @IBAction func onGoSelected(_ sender: AnyObject) {
        showLoadingIndicator()
        
        currentRoute.routeId = routeIdField.stringValue
        
        if isInternalCheckbox.state == NSOnState {
            currentRoute.isInternal = true
        } else {
            currentRoute.isInternal = false
        }
        
        currentRoute.routeDisplayName = routeDisplayNameField.stringValue
        currentRoute.routeDescription = routeDescriptionField.stringValue
        currentRoute.requestMethod = requestMethodField.stringValue.uppercased()
        currentRoute.requestUri = requestUriField.stringValue.lowercased()
        currentRoute.requestOverrideRouteId = requestRouteOverrideField.stringValue
        currentRoute.requestQueryStringContains = requestQueryStringContainsField.stringValue
        currentRoute.requestQueryStringEquals = requestQueryStringEqualsField.stringValue
        currentRoute.requestBodyJsonPath = requestBodyJsonPathField.stringValue
        currentRoute.requestBodyJsonPathContains = requestBodyJsonPathContainsField.stringValue
        currentRoute.requestBodyJsonPathEquals = requestBodyJsonPathEqualsField.stringValue
        currentRoute.responseStatusCode = UInt(responseStatusCodeField.stringValue) ?? 0
        currentRoute.responseDelay = UInt(responseDelayField.stringValue) ?? 0
        currentRoute.responseControllerId = responseControllerIdField.stringValue
        
        if ((jsonContentField.string?.characters.count)! > 0) {
            currentRoute.responseJsonFile = "\(currentRoute.routeId).json"
        } else {
            currentRoute.responseJsonFile = ""
        }
        
        let routeJson = currentRoute.toJson()
        
        if isEditingNewMock {
            createNewRoute(json: routeJson)
        } else {
            updateExistingRoute(json: routeJson)
        }
        
        hideLoadingIndicator()
    }
    
    @IBAction func onPushMockSelected(_ sender: NSButton) {
        Mockifer.pushMock(currentRoute.routeId, times: 1)
        reloadActiveMocks()
    }
    
    @IBAction func onClearActiveMocksSelected(_ sender: NSButton) {
        Mockifer.clearActiveMocks()
        reloadActiveMocks()
    }
    
    @IBAction func onResetServerSelected(_ sender: NSButton) {
        Mockifer.reset()
        reloadActiveMocks()
        reloadRoutes()
        startNewRoute()
    }
    
    @IBAction func onInternalToggleSelected(_ sender: NSButton) {
        if isEditingNewMock {
            lockPushMockButton()
        }
        
        if isInternalCheckbox.state == NSOffState {
            unlockPushMockButton()
        } else {
            lockPushMockButton()
        }
    }
    
    func updateExistingRoute(json: String) {
        if (Mockifer.updateExistingRoute(json, withResponseJson: jsonContentField.string ?? "")) {
            isEditingNewMock = false
            tableView.deselectAll(nil)
            Mockifer.reset()
            reloadRoutes()
            return
        }
        
        let alert = NSAlert()
        alert.messageText = "Update error"
        alert.informativeText = "The route could not be updated. Make sure you are updating a route with an existing route id."
        alert.alertStyle = NSAlertStyle.warning
        alert.addButton(withTitle: "OK")
        alert.runModal()
    }
    
    func createNewRoute(json: String) {
        if (Mockifer.createNewRoute(json, withResponseJson: jsonContentField.string ?? "")) {
            isEditingNewMock = false
            tableView.deselectAll(nil)
            Mockifer.reset()
            reloadRoutes()
            startNewRoute()
            return
        }
        
        let alert = NSAlert()
        alert.messageText = "Create error"
        alert.informativeText = "The route could not be created. Make sure you have entered a route id that isn't already in use."
        alert.alertStyle = NSAlertStyle.warning
        alert.addButton(withTitle: "OK")
        alert.runModal()
    }

    @IBAction func searchFieldChanged(_ sender: NSSearchField) {
        refreshRoutes()
    }
    
    func refreshRoutes() -> Void {
        let searchText = searchField.stringValue.uppercased()
        
        filteredRoutes = allRoutes.filter({ (dto) -> Bool in
            if searchText.characters.count == 0 {
                return true
            }
            
            return dto.searchableText.contains(searchText)
        })
        
        routesController.content = filteredRoutes
    }
    
    func refreshActiveMockRoutes() {
        activeMocksController.content = activeMockRoutes
    }
    
    func tableViewSelectionDidChange(_ notification: Notification) {
        if let it = notification.object as? NSTableView {
            let selections = it.selectedRowIndexes.map { Int($0) }
            if selections.count > 0 {
                handleRouteSelected(route: filteredRoutes[selections[0]])
            }
        }
    }
    
    func handleRouteSelected(route: Route) -> Void {
        isEditingNewMock = false
        currentRoute = route.copyOf()
        populateRouteDetails()
    }
    
    func populateRouteDetails() {
        routeIdField.stringValue = currentRoute.routeId
        
        if isEditingNewMock {
            unlockRouteIdField()
            pushMockButton.isHidden = true
        } else {
            lockRouteIdField()
            pushMockButton.isHidden = false
            if currentRoute.isInternal {
                isInternalCheckbox.state = NSOnState
                lockPushMockButton()
            } else {
                isInternalCheckbox.state = NSOffState
                unlockPushMockButton()
            }
        }
        
        routeDisplayNameField.stringValue = currentRoute.routeDisplayName
        routeDescriptionField.stringValue = currentRoute.routeDescription
        
        requestMethodField.stringValue = currentRoute.requestMethod
        requestUriField.stringValue = currentRoute.requestUri
        requestRouteOverrideField.stringValue = currentRoute.requestOverrideRouteId
        requestQueryStringContainsField.stringValue = currentRoute.requestQueryStringContains
        requestQueryStringEqualsField.stringValue = currentRoute.requestQueryStringEquals
        
        requestBodyJsonPathField.stringValue = currentRoute.requestBodyJsonPath
        requestBodyJsonPathContainsField.stringValue = currentRoute.requestBodyJsonPathContains
        requestBodyJsonPathEqualsField.stringValue = currentRoute.requestBodyJsonPathEquals
        
        if currentRoute.responseStatusCode > 0 {
            responseStatusCodeField.stringValue = "\(currentRoute.responseStatusCode)"
        } else {
            responseStatusCodeField.stringValue = ""
        }

        if currentRoute.responseDelay > 0 {
            responseDelayField.stringValue = "\(currentRoute.responseDelay)"
        } else {
            responseDelayField.stringValue = ""
        }
        
        responseControllerIdField.stringValue = currentRoute.responseControllerId
        
        if currentRoute.responseJsonFile.characters.count == 0 {
            jsonContentField.string = ""
        } else {
            jsonContentField.string = Mockifer.loadRouteJsonFileContent(currentRoute.routeId);
        }
    }
    
    func textDidChange(_ notification: Notification) {
        guard let textView = notification.object as? NSTextView else { return }
        
        if (textView == jsonContentField) {
            jsonContentChanged(content: textView.string ?? "")
        }
    }
    
    func jsonContentChanged(content: String) {
        if content.characters.count == 0 {
            currentRoute.responseJsonFile = ""
            jsonContentFileField.stringValue = ""
            return
        }
        
        currentRoute.responseJsonFile = "\(routeIdField.stringValue).json"
        jsonContentFileField.stringValue = "data/mocks/\(currentRoute.responseJsonFile)"
    }
    
    func startNewRoute() {
        tableView.deselectAll(nil)
        isEditingNewMock = true
        currentRoute = Route()
        populateRouteDetails()
        unlockRouteIdField()
        routeIdField.becomeFirstResponder()
    }
    
    @IBAction func onCreateNewSelected(_ sender: NSButton) {
        startNewRoute()
    }
    
    @IBAction func onDeleteRouteSelected(_ sender: NSButton) {
        if isEditingNewMock {
            startNewRoute()
            return
        }
        
        let alert = NSAlert()
        alert.messageText = "Delete route"
        alert.informativeText = "Are you sure you want to delete route with id '\(currentRoute.routeId)? This action cannot be undone."
        alert.alertStyle = NSAlertStyle.warning
        alert.addButton(withTitle: "Delete")
        alert.addButton(withTitle: "Cancel")
        let selection = alert.runModal()
        
        if (selection == NSAlertFirstButtonReturn) {
            deleteCurrentRoute()
        }
    }
    
    func lockPushMockButton() {
        pushMockButton.isEnabled = false
    }
    
    func unlockPushMockButton() {
        pushMockButton.isEnabled = true
    }
    
    func lockRouteIdField() {
        routeIdField.isEnabled = false
    }
    
    func unlockRouteIdField() {
        routeIdField.isEnabled = true
    }
    
    func deleteCurrentRoute() {
        if Mockifer.deleteRoute(currentRoute.routeId) {
            tableView.deselectAll(nil)
            Mockifer.reset()
            reloadRoutes()
            startNewRoute()
            return
        }
        
        let alert = NSAlert()
        alert.messageText = "Delete error"
        alert.informativeText = "The route could not be deleted. Make sure you are deleting an existing route id."
        alert.alertStyle = NSAlertStyle.warning
        alert.addButton(withTitle: "OK")
        alert.runModal()
    }
    
    override func controlTextDidChange(_ obj: Notification) {
        guard let target = obj.object as? NSTextField else {
            return
        }
        
        if target == routeIdField {
            routeIdField.stringValue = idFormatter.filter(input: routeIdField.stringValue)
        }
        
        if target == requestRouteOverrideField {
            requestRouteOverrideField.stringValue = idFormatter.filter(input: requestRouteOverrideField.stringValue)
        }

        if target == responseControllerIdField {
            responseControllerIdField.stringValue = idFormatter.filter(input: responseControllerIdField.stringValue)
        }

        if target == responseStatusCodeField {
            responseStatusCodeField.stringValue = statusCodeFormatter.filter(input: responseStatusCodeField.stringValue)
        }
        
        if target == responseDelayField {
            responseDelayField.stringValue = responseDelayFormatter.filter(input: responseDelayField.stringValue)
        }
    }
}
