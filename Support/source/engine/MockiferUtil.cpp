/*
 * =================================================
 * Mockifer License
 * =================================================
 *
 * MIT License
 *
 * Copyright (c) 2017 Marcel Braghetto
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "MockiferUtil.h"

USING_NS_MOCKIFER;

string MockiferUtil::absolutePath(const string &path) {
    return string(realpath(path.c_str(), NULL));
}

bool MockiferUtil::pathExists(const string &path, const bool &isDirectory) {
    struct stat info;
    
    // Nope, can't find or access the given path...
    if (stat(path.c_str(), &info) != 0) {
        return false;
    }
    
    // See if we found a directory or file as required.
    return info.st_mode & (isDirectory ? S_IFDIR : S_IFREG);
}

pair<bool, string> MockiferUtil::loadTextFile(const string &path) {
    ifstream inputFile {path};
    if (inputFile.good()) {
        string result {istreambuf_iterator<char>(inputFile), istreambuf_iterator<char>()};
        return make_pair(true, result);
    }

    return make_pair(false, "");
}

bool MockiferUtil::saveTextFile(const string &path, const string &content) {
    ofstream outputFile {path};
    if (outputFile.good()) {
        outputFile << content;
        outputFile.close();
        return true;
    }
    
    return false;
}

bool MockiferUtil::deleteFile(const string &path) {
    return remove(path.c_str()) == 0;
}

MockiferManifest MockiferUtil::loadManifest(const string &contentPath) {
    MockiferManifest manifest;
    auto manifestFilePath = contentPath + "/manifest.json";
    
    ifstream manifestFile { manifestFilePath };
    
    if (manifestFile.good()) {
        string jsonData { istreambuf_iterator<char>(manifestFile), istreambuf_iterator<char>() };
        
        Json::Value root;
        Json::Reader reader;
        
        bool parsingSuccessful = reader.parse(jsonData, root);
        if (parsingSuccessful) {
            for (const auto &bootStrapElement : root.get("bootstrap_javascript", "[]")) {
                manifest.javascriptBootstrapFiles.push_back(bootStrapElement.asString());
            }

            for (const auto &fileType : root.get("binary_response_file_types", "[]")) {
                manifest.binaryResponseFileTypes.push_back(fileType.asString());
            }
        } else {
            LOGE("FATAL: Could not parse 'manifest.json'.");
        }
    } else {
        LOGE("FATAL: Could not find 'manifest.json'.");
    }
    
    return manifest;
}

vector<string> MockiferUtil::split(const string &input, const char &delimiter) {
    vector<string> tokens;
    size_t start = 0, end = 0;
    
    while ((end = input.find(delimiter, start)) != string::npos) {
        if (end != start) {
            tokens.push_back(input.substr(start, end - start));
        }
        start = end + 1;
    }
    
    if (end != start) {
        tokens.push_back(input.substr(start));
    }
    
    return tokens;
}

string MockiferUtil::toLowerCase(string source) {
    auto result = source;
    locale loc;
    
    for (auto i = 0; i < result.length(); i++) {
        result[i] = tolower(result[i], loc);
    }
    
    return result;
}
