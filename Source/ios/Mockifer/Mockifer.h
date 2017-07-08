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

#import <UIKit/UIKit.h>

FOUNDATION_EXPORT double MockiferVersionNumber;
FOUNDATION_EXPORT const unsigned char MockiferVersionString[];

@interface Mockifer : NSObject

/*
 Start Mockifer with default settings:
 
 - javascript content will be mapped to a relative path of 'mockifer-js'.
 - server port will default to 8502.
 
 */
+ (void) start;

/*
 Start Mockifer with the default content path but with the given http server port.
 */
+ (void) startOnPort: (nonnull NSString *) port;

/*
 Set a global delay to apply for each request to simulate network timings. Calling 'reset' clears this value.
 */
+ (void) setGlobalResponseDelay: (uint) responseDelayMillis;

/*
 Stop and destroy the Mockifer server.
 */
+ (void) stop;

/*
 Reset the Mockifer server session to its default state.
 */
+ (void) reset;

/*
 Clear any active mocks that are pending to intercept requests.
 */
+ (void) clearActiveMocks;

/*
 Push a collection of mock route ids to activate when evaluating incoming requests.
 Each mock will be added once and if triggered will be consumed and removed.
 */
+ (void) pushMocks: (nonnull NSArray<NSString*>*) mockRouteIds;

/*
 Push a single mock with the given route id to activate when evaluating incoming requests.
 The mock will be added once and if triggered will be consumed and removed.
 */
+ (void) pushMock: (nonnull NSString*) mockRouteId;

/*
 Push a single mock with the given route id to activate when evaluating incoming requests.
 The mock will be added the number of times specified and can be triggered that many times
 before being consumed.
 */
+ (void) pushMock: (nonnull NSString*) mockRouteId times: (uint) times;

@end
