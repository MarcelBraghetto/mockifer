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

#import "Mockifer.h"
#import "MockiferServer.h"

@implementation Mockifer

    + (BOOL) directoryExists: (NSString*) path {
        BOOL isDirectory = NO;
        BOOL pathExists = [[NSFileManager defaultManager] fileExistsAtPath:path isDirectory:&isDirectory];
        
        return pathExists && isDirectory;
    }

    + (void) start {
        NSString* port = @"8501";
        
        NSString *contentPath = @"";
        NSMutableArray *syncPaths = [NSMutableArray new];
        
        // At the very minimum, a -contentpath "/some/path/to/mockifer-js" must be provided.
        //
        // In addition, editing data through this application can sync data changes to routes
        // and mock files by passing launch arguments when opening the app in the form of:
        //
        // -syncpath "/some/path/to/mockifer-js1" -syncpath "/some/path/to/mockifer-js2"
        //
        // All 'sync paths' that are included will receive changes made while in this app.
        NSArray *args = [[NSProcessInfo processInfo] arguments];
        for (int i = 0; i < [args count]; i++) {
            if ([[args objectAtIndex:i] isEqualToString:@"-contentpath"]) {
                if (i < [args count] - 1) {
                    contentPath = [args objectAtIndex:i + 1];
                }
            }
            
            if ([[args objectAtIndex:i] isEqualToString:@"-syncpath"]) {
                if (i < [args count] - 1) {
                    [syncPaths addObject:[args objectAtIndex:i + 1]];
                }
            }
        }
        
        // Check if we are running at the root level and can reach the source code for the content
        NSURL *appFolder = [[[NSBundle mainBundle] bundleURL] URLByDeletingLastPathComponent];
        NSURL *contentSourceFolder = [appFolder URLByAppendingPathComponent:@"content/src/main/resources/mockifer"];
        
        NSString *contentSourcePath = [contentSourceFolder path];
        if ([Mockifer directoryExists:contentSourcePath]) {
            [syncPaths addObject:contentSourcePath];
        }

        // Check if there is compiled content that can be marked to sync with
        NSURL *compiledContentFolder = [appFolder URLByAppendingPathComponent:@"products/content/editor/mockifer-js"];
        
        NSString *compiledContentPath = [compiledContentFolder path];
        if ([Mockifer directoryExists:compiledContentPath]) {
            contentPath = compiledContentPath;
        }
        
        if ([contentPath length] == 0) {
            NSLog(@"MOCKIFER ERROR! NO CONTENT PATH FOUND!");
            return;
        }
        
        // Boot the server
        mockifer::MockiferServer::start([contentPath UTF8String], [port UTF8String]);
        
        // Register the paths to 'sync' data to.
        for (NSString *syncPath : syncPaths) {
            mockifer::MockiferServer::addDataSyncPath([syncPath UTF8String]);
        }
    }

    + (void) pushMock:(NSString *)mockRouteId times:(int)times {
        mockifer::MockiferServer::pushMock([mockRouteId UTF8String], times);
    }

    + (void) clearActiveMocks {
        mockifer::MockiferServer::clearActiveMocks();
    }

    + (void) setGlobalResponseDelay:(uint)responseDelayMillis {
        mockifer::MockiferServer::setGlobalResponseDelay(responseDelayMillis);
    }

    + (void) stop {
        mockifer::MockiferServer::stop();
    }

    + (void) reset {
        mockifer::MockiferServer::reset();
    }

    + (bool) updateExistingRoute:(NSString *)routeJson withResponseJson: (NSString*) responseJson {
        return mockifer::MockiferServer::updateExistingRoute([routeJson UTF8String], [responseJson UTF8String]);
    }

    + (bool) createNewRoute:(NSString *)routeJson withResponseJson: (NSString*) responseJson{
        return mockifer::MockiferServer::createNewRoute([routeJson UTF8String], [responseJson UTF8String]);
    }

    + (bool) deleteRoute:(NSString *)routeId {
        return mockifer::MockiferServer::deleteRoute([routeId UTF8String]);
    }

    + (NSString*) getBaseUrl {
        return @(mockifer::MockiferServer::getBaseUrl().c_str());
    }

    + (nonnull NSString*) loadRouteJsonFileContent:(NSString *)routeId {
        return @(mockifer::MockiferServer::loadRouteJsonFileContent([routeId UTF8String]).c_str());
    }

    /*
     This method causes all logging output to end up in the 'Console.app' application, otherwise it wouldn't
     appear there. When viewing the Console output, type 'Mockifer' as the filter and right click on a logging output and choose 'Hide Library 'CFNetwork'.
     */
    int printf(const char * __restrict format, ...) {
        va_list args;
        va_start(args, format);
        NSLogv([NSString stringWithUTF8String:format], args);
        va_end(args);
        return 1;
    }

@end
