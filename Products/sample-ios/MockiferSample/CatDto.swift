import Foundation

class CatDto : NSObject {
    dynamic var id = ""
    dynamic var name = ""
    dynamic var age = 0
    
    override init() {
        super.init()
    }
    
    init(source: Dictionary<String, AnyObject>) {
        id = source["id"] as! String
        name = source["name"] as! String
        age = source["age"] as! Int
        super.init()
    }
}
