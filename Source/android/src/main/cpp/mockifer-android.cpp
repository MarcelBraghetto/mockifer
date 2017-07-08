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

#include <jni.h>
#include "MockiferServer.h"

extern "C"
JNIEXPORT void JNICALL Java_mockifer_Mockifer_nativeStartServer(JNIEnv* env, jobject, jstring jContentPath, jstring jPort) {
    const char *contentPath = env->GetStringUTFChars(jContentPath, JNI_FALSE);
    const char *port = env->GetStringUTFChars(jPort, JNI_FALSE);

    mockifer::MockiferServer::start(contentPath, port);

    env->ReleaseStringUTFChars(jContentPath, contentPath);
    env->ReleaseStringUTFChars(jPort, port);
}

extern "C"
JNIEXPORT void JNICALL Java_mockifer_Mockifer_nativeSetGlobalResponseDelay(JNIEnv* env, jobject, jint jDelay) {
    mockifer::MockiferServer::setGlobalResponseDelay((uint) jDelay);
}

extern "C"
JNIEXPORT void JNICALL Java_mockifer_Mockifer_nativeStopServer(JNIEnv* env, jobject) {
    mockifer::MockiferServer::stop();
}

extern "C"
JNIEXPORT void JNICALL Java_mockifer_Mockifer_nativeResetServer(JNIEnv* env, jobject) {
    mockifer::MockiferServer::reset();
}

extern "C"
JNIEXPORT void JNICALL Java_mockifer_Mockifer_nativePushMock(JNIEnv* env, jobject, jstring jMockRouteId, jint jTimes) {
    const char *mockRouteId = env->GetStringUTFChars(jMockRouteId, JNI_FALSE);
    int times = (int) jTimes;

    mockifer::MockiferServer::pushMock(mockRouteId, times);

    env->ReleaseStringUTFChars(jMockRouteId, mockRouteId);
}

extern "C"
JNIEXPORT void JNICALL Java_mockifer_Mockifer_nativeClearActiveMocks(JNIEnv* env, jobject) {
    mockifer::MockiferServer::clearActiveMocks();
}
