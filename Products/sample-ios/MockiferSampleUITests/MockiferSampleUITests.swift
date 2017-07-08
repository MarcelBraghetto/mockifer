import XCTest
import Mockifer

class MockiferSampleUITests: XCTestCase {
    lazy var app: XCUIApplication = XCUIApplication()
        
    override func setUp() {
        super.setUp()
        continueAfterFailure = false
        app.launch()
        Mockifer.reset()
    }
    
    func testEditCat() {
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
        app.navigationBars.buttons["Mock Error"].tap()
        
        app.alerts["Network Error"].buttons["OK"].tap()
    }
}
