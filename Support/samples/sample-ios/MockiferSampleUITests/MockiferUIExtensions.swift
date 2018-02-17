import Foundation
import XCTest

extension XCUIElement {
    func setText(text: String, application: XCUIApplication) {
        UIPasteboard.general.string = text
        self.tap()
        self.doubleTap()
        application.menuItems["Paste"].tap()
    }
}
