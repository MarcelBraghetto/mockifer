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
+ (void) start {
    [Mockifer startOnPort:@"8502"];
}

+ (uint) startOnDynamicPort {
    NSString* jsPath = [NSString stringWithFormat:@"%@/%@", [[NSBundle mainBundle] bundlePath], @"mockifer-js"];
    return mockifer::MockiferServer::startOnDynamicPort([jsPath UTF8String]);
}

+ (void) startWithContentPath: (nonnull NSString *) contentPath {
    NSString *port = @"8502";
    mockifer::MockiferServer::start([contentPath UTF8String], [port UTF8String]);
}

+ (void) setCommandUrl: (nonnull NSString *) baseUrl onPort: (uint) port {
    mockifer::MockiferServer::setCommandUrl([baseUrl UTF8String], port);
}

+ (void) startWithPathPort: (nonnull NSString *) contentPath onPort: (uint) customPort {
    mockifer::MockiferServer::startCustom([contentPath UTF8String], customPort);
}

+ (void) startOnPort:(nonnull NSString *)port {
    NSString* jsPath = [NSString stringWithFormat:@"%@/%@", [[NSBundle mainBundle] bundlePath], @"mockifer-js"];
    mockifer::MockiferServer::start([jsPath UTF8String], [port UTF8String]);
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

+ (void) clearActiveMocks {
    mockifer::MockiferServer::clearActiveMocks();
}

+ (void) pushMocks:(nonnull NSArray<NSString *> *)mockRouteIds {
    for (NSString* mockRouteId : mockRouteIds) {
        [Mockifer pushMock:mockRouteId];
    }
}

+ (void) pushMock:(nonnull NSString *)mockRouteId {
    [Mockifer pushMock:mockRouteId times:1];
}

+ (void) pushMock:(nonnull NSString *)mockRouteId times:(uint)times {
    mockifer::MockiferServer::pushMock([mockRouteId UTF8String], times);
}

@end

