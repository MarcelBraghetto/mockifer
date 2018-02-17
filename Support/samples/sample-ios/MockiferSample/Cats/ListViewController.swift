import UIKit

class ListViewController: UIViewController, UITableViewDataSource, UITableViewDelegate {
    @IBOutlet weak var tableView: UITableView!
    @IBOutlet weak var mockResetButton: UIBarButtonItem!
    @IBOutlet weak var mockErrorButton: UIBarButtonItem!
    
    private var cats = [CatDto]()
    private var imageCache: [String: UIImage] = [:]
    
    override func viewDidLoad() {
        super.viewDidLoad()
        tableView.accessibilityIdentifier = "CatsTableView"
        
        if (!MockiferUtil.isMockApp) {
            self.navigationItem.rightBarButtonItems = nil
        }
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        loadCats()
    }
    
    func loadCats() {
        Requester.load(method: "GET", uri: "/cats", body: nil) { (data, response, error) in
            guard error == nil else {
                print("ERROR: \(error.debugDescription)")
                return
            }
            
            guard let data = data else {
                self.refreshList()
                return
            }
            
            if let httpResponse = response as? HTTPURLResponse {
                if (httpResponse.statusCode >= 400) {
                    let responseText = String(data: data, encoding: .utf8) as String! ?? ""
                    self.showNetworkErrorAlert(message: "Status code: \(httpResponse.statusCode)\n\(responseText)")
                }
            }
            
            self.cats = [CatDto]()
            
            do {
                guard let catsData = try JSONSerialization.jsonObject(with: data, options: .allowFragments) as? [String : AnyObject] else {
                    self.refreshList()
                    return
                }
                
                if let results = catsData["cats"] as? [[String : AnyObject]] {
                    self.cats = results.map {
                        return CatDto(source: $0)
                    }
                }
            } catch _ {
                
            }
            
            self.refreshList()
        }
    }
    
    func refreshList() {
        tableView.reloadData()
    }
    
    func showNetworkErrorAlert(message: String) {
        let alertController = UIAlertController(title: "Network Error", message: message, preferredStyle: .alert)
        alertController.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
        self.present(alertController, animated: true, completion: nil)
    }
    
    @IBAction func onResetSelected(_ sender: UIBarButtonItem) {
        MockiferUtil.reset()
        loadCats()
    }
    
    @IBAction func onMockErrorSelected(_ sender: UIBarButtonItem) {
        // Push a mock that deliberately returns a 500 error
        MockiferUtil.pushMock(routeId: "mocks.cats.getcats.error")
        loadCats()
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        tableView.deselectRow(at: indexPath, animated: true)
        let detailsViewController = storyboard?.instantiateViewController(withIdentifier: "CatDetails") as! DetailsViewController
        detailsViewController.cat = cats[indexPath.row];
        navigationController?.pushViewController(detailsViewController, animated: true)
    }
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return cats.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "CatCell", for: indexPath)
        
        let cat = cats[indexPath.row];
        cell.textLabel?.text = "\(cat.name)"
        cell.detailTextLabel?.text = "Id: \(cat.id), Age: \(cat.age)"
        
        cell.imageView?.image = imageCache[cat.image]
        
        if (cell.imageView?.image == nil) {
            Requester.load(method: "GET", uri: cat.image, body: nil) { (data, response, error) in
                // This code is pretty basic, probably needs some resilience to cell reuse, but good enough for this demo app.
                if error == nil, let targetCell = tableView.cellForRow(at: indexPath), let data = data {
                    let imageData = UIImage(data: data)
                    self.imageCache[cat.image] = imageData
                    targetCell.imageView?.image = imageData
                    tableView.reloadRows(at: [indexPath], with: .none)
                }
            }
        }
        
        return cell
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        imageCache.removeAll()
    }
}

