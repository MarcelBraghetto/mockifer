#!/bin/bash

# Make sure you have built the iOS Mockifer Framework at least once before running this script.

# You might need different simulator 'destination' settings.
cd Products/sample-ios
xcodebuild test -project MockiferSample.xcodeproj -scheme MockiferSample -destination 'platform=iOS Simulator,name=iPhone 5,OS=10.3'