#!/bin/bash

# Make sure you have built the iOS Mockifer Framework at least once before running this script.

# You'll need to configure your own device name here - this is my crusty old first gen iPad Mini!
DEVICE_NAME="Marcel's iPad Mini"

cd Products/sample-ios
xcodebuild test -project MockiferSample.xcodeproj -scheme MockiferSample -destination "platform=iOS,name=${DEVICE_NAME}"