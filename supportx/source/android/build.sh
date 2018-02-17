#!/bin/bash

echo "Building Android .AAR library.."
./gradlew mockiferBuildFramework

echo "Build complete - Android .AAR library can be found in the 'products/libs/android' directory."