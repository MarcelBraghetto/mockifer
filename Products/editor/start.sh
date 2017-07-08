#!/bin/bash -e
# Note: This script will not build the application, if you need to build it then run: ./gradlew mockiferBuildEditor from the root directory.

CURRENT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CONTENTPATH_RELATIVE="$CURRENT_DIR/../js/mockifer-js"
SYNCPATH_RELATIVE="$CURRENT_DIR/../../Source/js/src/main/resources/mockifer"

CONTENTPATH="`cd "${CONTENTPATH_RELATIVE}";pwd`"
SYNCPATH="`cd "${SYNCPATH_RELATIVE}";pwd`"

echo "Starting Mockifer with:"
echo "-> Content path: $CONTENTPATH"
echo "-> Sync path:    $SYNCPATH"

open -a "${CURRENT_DIR}/MockiferEditor.app" --args -contentpath "$CONTENTPATH" -syncpath "$SYNCPATH"