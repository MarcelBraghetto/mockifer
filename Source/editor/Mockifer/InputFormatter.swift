import Foundation

class InputFormatter {
    private let permittedCharacterSet: CharacterSet
    private let maxLength: Int
    
    init(permittedCharacters: String, withMaxLength maxLength: Int) {
        self.maxLength = maxLength
        permittedCharacterSet = CharacterSet(charactersIn: permittedCharacters).inverted
    }
    
    func filter(input: String?) -> String {
        guard let input = input else {
            return ""
        }
        
        var filtered = (input.components(separatedBy: permittedCharacterSet)).joined(separator: "")
        
        if filtered.characters.count > maxLength {
            filtered = filtered.substring(to: filtered.index(filtered.startIndex, offsetBy: maxLength))
        }
        
        return filtered
    }
}
