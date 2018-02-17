#!/bin/bash

echo "Building content.."
cd content
./gradlew mockiferBuildContent

echo "Build complete - content can be found in the 'products/content' directory"