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

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "MockiferServer.h"

using std::cout;
using std::string;

static string ERROR_TAG = "✕ MOCKIFER-CONSOLE: ";
static string TAG = "✔ MOCKIFER-CONSOLE: ";
static string LINEBREAK = "\n";

bool pathExists(string path, bool isDirectory) {
    struct stat info;
    
    // Nope, can't find or access the given path...
    if (stat(path.c_str(), &info) != 0) {
        return false;
    }
    
    // See if we found a directory or file as required.
    return info.st_mode & (isDirectory ? S_IFDIR : S_IFREG);
}

bool validateContentPath(string contentPath) {
    if (!pathExists(contentPath, true)) {
        cout << ERROR_TAG << "Content directory not found or inaccesible: " << contentPath << LINEBREAK;
        return false;
    }
    
    if (!pathExists(contentPath + "/manifest.json", false)) {
        cout << ERROR_TAG << "Content directory missing 'manifest.json'. Check your content" << LINEBREAK;
        return false;
    }
    
    if (!pathExists(contentPath + "/api.js", false)) {
        cout << ERROR_TAG << "Content directory missing 'api.js'. Check your content" << LINEBREAK;
        return false;
    }
    
    if (!pathExists(contentPath + "/routes.json", false)) {
        cout << ERROR_TAG << "Content directory missing 'routes.json'. Check your content" << LINEBREAK;
        return false;
    }
    
    return true;
}

int main(int argc, char * argv[]) {
    int opt = 0;
    string contentPath{""};
    string port{""};
    
    while ((opt = getopt(argc, argv, "p:d:")) != -1) {
        if (opt == 'p') {
            port = string(optarg);
            continue;
        }
        
        if (opt == 'd') {
            contentPath = string(optarg);
            continue;
        }
    }
    
    if (port == "") {
        port = "8504";
    }
    
    if (contentPath == "") {
        // We can share the same content that is compiled for the editor application.
        contentPath = "./products/content/editor/mockifer-js";
    }
    
    if (!validateContentPath(contentPath)) {
        return 1;
    }
    
    cout << TAG << "Booting Mockifer..." << LINEBREAK;
    
    mockifer::MockiferServer::start(contentPath, port);
    
    for(;;) {
        // We are just gonna spin here pretty much forever until we are quit...
    }

    return 0;
}

