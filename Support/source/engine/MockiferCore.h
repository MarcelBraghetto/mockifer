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

#pragma once

#define NS_MOCKIFER_BEGIN   namespace mockifer {
#define NS_MOCKIFER_END     }
#define USING_NS_MOCKIFER   using namespace mockifer
#define NS_MOCKIFER         ::mockifer
#define USING_NS_STD        using namespace std

#include <string>
#include <iostream>
#include "civetweb.h"
#include "duktape.h"
#include "json/json/json.h"
#include "json/json/json-forwards.h"

// DukTape:  http://duktape.org/
// Json CPP: https://github.com/open-source-parsers/jsoncpp
// CivitWeb: https://github.com/civetweb/civetweb
// --> https://github.com/civetweb/civetweb/blob/master/docs/UserManual.md

#ifdef ANDROID

#include <android/log.h>
#define LOG_TAG "MOCKIFER"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO   , LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN   , LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , LOG_TAG,__VA_ARGS__)
#define LOGSIMPLE(...)

#elif MOCKIFER_EDITOR

#define LOGV(...) printf(__VA_ARGS__);
#define LOGD(...) printf(__VA_ARGS__);
#define LOGI(...) printf(__VA_ARGS__);
#define LOGW(...) printf(__VA_ARGS__);
#define LOGE(...) printf(__VA_ARGS__);
#define LOGSIMPLE(...) printf(__VA_ARGS__);

#else

#include <stdio.h>
#define LOG_TAG "✪ MOCKIFER"
#define LOGV(...) printf("%s: ", LOG_TAG); printf(__VA_ARGS__); printf("\n");
#define LOGD(...) printf("%s: ", LOG_TAG); printf(__VA_ARGS__); printf("\n");
#define LOGI(...) printf("%s: ", LOG_TAG); printf(__VA_ARGS__); printf("\n");
#define LOGW(...) printf("%s: * Warning -> ", LOG_TAG); printf(__VA_ARGS__); printf("\n");
#define LOGE(...) printf("%s: ** Error -> ", LOG_TAG);printf(__VA_ARGS__); printf("\n");
#define LOGSIMPLE(...) printf(" ");printf(__VA_ARGS__);

#endif

USING_NS_STD;
