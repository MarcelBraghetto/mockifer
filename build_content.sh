#!/usr/bin/env bash

set -eu

# Alias the 'pushd' command and have it send its output to the abyss ...
pushd() {
	command pushd "$@" > /dev/null
}

# Alias the 'popd' command and have it send its output to the abyss ...
popd() {
	command popd "$@" > /dev/null
}

# Run from script folder
pushd $(cd `dirname $0` && pwd)

	echo "Building content.."
	cd content
	./gradlew mockiferBuildContent

	echo "Build complete - content can be found in the 'products/content' directory"

popd
