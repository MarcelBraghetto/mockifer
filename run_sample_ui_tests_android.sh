#!/bin/bash

# Make sure you have built the Android Mockifer Framework at least once before running this script.

# Make sure you have a plugged in Android device with USB debugging enabled.
cd Products/sample-android
./gradlew connectedDebugAndroidTest