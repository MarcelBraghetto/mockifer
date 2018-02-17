# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.4.1)

# Preprocessor definitions (currently need a flag for Json CPP to work on NDK)
add_definitions(-DJSONCPP_NO_LOCALE_SUPPORT)

# C/C++ header file search directories.
include_directories(
../engine/
../engine/duktape/
../engine/json/json/
../engine/civetweb/
)

add_library(
mockifer
SHARED

# List each of the C/C++ source files to include in compilation.
# This also drives the 'CPP source set' for C/C++ code within Android Studio.

# Mockifer Android Library C++
src/main/cpp/mockifer-android.cpp

# Mockifer C/C++ added via relative paths so other targets can share the same code.
../engine/duktape/duktape.c
../engine/json/jsoncpp.cpp
../engine/civetweb/civetweb.c
../engine/MockiferRequest.cpp
../engine/MockiferResponse.cpp
../engine/MockiferServer.cpp
../engine/MockiferRouteManager.cpp
../engine/MockiferRoute.cpp
../engine/MockiferUtil.cpp
../engine/MockiferManifest.cpp
)

find_library(
log-lib
log
)

target_link_libraries(
mockifer
${log-lib}
)
