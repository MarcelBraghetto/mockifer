import Foundation

class Requester {
    static func load(method: String, uri: String, body: String?, callback: @escaping (Data?, URLResponse?, Error?) -> Void) {
        let session = URLSession(configuration: URLSession.shared.configuration, delegate:nil, delegateQueue:OperationQueue.main)
        let encodedUri = uri.addingPercentEncoding(withAllowedCharacters: .urlQueryAllowed) ?? ""
        
        var request = URLRequest(url: URL(string: "\(ServerConfig.serverUrl):\(ServerConfig.serverPort)" + encodedUri)!)
        request.httpMethod = method
        
        if let body = body {
            request.httpBody = body.data(using: .utf8)
        }
        
        session.dataTask(with: request) { (data, response, error) in callback(data, response, error) }.resume()
    }
}
