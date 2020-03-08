import XCTest
import Mockifer

// Note: Don't forget to adjust the ServerConfig.serverPort to run automation against
// the self contained Mockifer instance. In a real project the server port should be
// configured automatically as part of your solution so you don't need to manually
// change it.
class MockiferSampleUITests: XCTestCase {
    static var hasSetupMockifer = false
    
    lazy var app: XCUIApplication = XCUIApplication()
    
    private func setupMockifer() {
        // Mockifer is embedded into the automation test target and runs its own server, so all that has to
        // happen is for the normal app to point at the embedded server, which by default runs on:
        // http://localhost:8502. This allows automation suites to run in a 'self contained' manner even in
        // a CI environment. The normal app itself doesn't know its pointing at a mock.
        if (!MockiferSampleUITests.hasSetupMockifer) {
            MockiferSampleUITests.hasSetupMockifer = true
            // The 'mockifer-js' content will have been bundled into the automation test target application
            let contentPath = "\(Bundle(for: type(of: self)).bundlePath)/mockifer-js"
            Mockifer.start(withContentPath: contentPath)
            // The 'ServerConfig' class is shared between the normal app and the automation target, in a
            // real situation you would have some way to configure the port based on some outside influence
            // rather than manually changing the code in that class to switch between ports.
            Mockifer.setCommandUrl("localhost", onPort: ServerConfig.serverPort)
        }
        
        Mockifer.reset()
    }
    
    override func setUp() {
        super.setUp()
        continueAfterFailure = false
        setupMockifer()
    }
    
    private func launchApp() {
        app.launch()
    }
    
    func testEditCat() {
        launchApp()
        
        // Tap on the first cat in the list
        let tableView = app.tables.containing(.table, identifier: "CatsTableView")
        tableView.cells.element(boundBy: 0).tap()
        
        // Replace the name of the cat
        let nameField = app.textFields["CatNameField"]
        XCTAssertEqual("Smish", nameField.value as! String)
        nameField.setText(text: "Updated kitty", application: app)
        
        // Trigger the save cat function
        app.buttons["SaveCatButton"].tap()
        
        // Reselect the first cat in the list and verify its name has changed
        tableView.cells.element(boundBy: 0).tap()
        XCTAssertEqual("Updated kitty", nameField.value as! String)
    }
    
    func testDeleteCat() {
        launchApp()
        
        // Verify the number of cats in the list
        let tableView = app.tables.containing(.table, identifier: "CatsTableView")
        XCTAssertTrue(tableView.cells.count == 4)
        
        // Select the first cat in the list
        tableView.cells.element(boundBy: 0).tap()
        
        // Trigger the 'delete cat' function
        app.buttons["DeleteCatButton"].tap()
        
        // Verify there are now 3 cats in the list
        XCTAssertTrue(tableView.cells.count == 3)
    }
    
    func testNetworkError() {
        // Push a mock that causes an error into the screen
        Mockifer.pushMock("mocks.cats.getcats.error")
        
        launchApp()
        
        app.alerts["Network Error"].buttons["OK"].tap()
    }
}
