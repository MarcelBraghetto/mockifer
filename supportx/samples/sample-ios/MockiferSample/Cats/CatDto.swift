import Foundation

class CatDto : NSObject {
    dynamic var id = ""
    dynamic var name = ""
    dynamic var age = 0
    dynamic var image = ""
    
    override init() {
        super.init()
    }
    
    init(source: Dictionary<String, AnyObject>) {
        id = source["id"] as! String
        name = source["name"] as! String
        age = source["age"] as! Int
        image = source["image"] as! String
        super.init()
    }
}
