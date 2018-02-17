import Foundation
import Mockifer

class MockiferUtil {
    static let isMockApp = true
    
    static func pushMock(routeId: String) {
        Mockifer.pushMock(routeId)
    }
    
    static func reset() {
        Mockifer.reset()
    }
}

