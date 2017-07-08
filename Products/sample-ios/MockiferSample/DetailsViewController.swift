import Foundation
import UIKit

class DetailsViewController: UIViewController {
    var cat = CatDto()
    
    @IBOutlet weak var catIdLabel: UILabel!
    @IBOutlet weak var catNameField: UITextField!
    @IBOutlet weak var catAgeField: UITextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        catIdLabel.text = cat.id
        catNameField.text = cat.name
        catAgeField.text = "\(cat.age)"
    }
    
    @IBAction func onDeleteCatSelected(_ sender: UIButton) {
        Requester.load(method: "DELETE", uri: "/cats/\(cat.id)", body: nil) { (data, response, error) in
            _ = self.navigationController?.popViewController(animated: true)
        }
    }
    
    @IBAction func onSaveCatSelected(_ sender: UIButton) {
        let requestBody = "{\"id\":\"\(cat.id)\",\"name\":\"\(catNameField.text!)\",\"age\":\(catAgeField.text!)}"
        
        Requester.load(method: "PUT", uri: "/cats", body: requestBody) { (data, response, error) in
            _ = self.navigationController?.popViewController(animated: true)
        }
    }    
}
