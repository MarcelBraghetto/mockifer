set -e

echo ''
echo '======================================================='
echo 'MOCKIFER: EDITOR BUILD STARTING'
echo '======================================================='
echo ''

PRODUCT_PATH="./../../../MockiferEditor.app"

echo 'Mockifer: Editor: Cleaning build folder...'
if [ -d "./build" ]; then
rm -rf "./build"
fi

echo 'Mockifer: Editor: Building target...'
xcodebuild -target "MockiferEditor"  > /dev/null

echo 'Mockifer: Editor: Removing existing app...'
if [ -d "${PRODUCT_PATH}" ]; then
rm -rf "${PRODUCT_PATH}"
fi

echo 'Mockifer: Editor: Copying app to output path...'
cp -r "./build/Release/MockiferEditor.app" "${PRODUCT_PATH}"

echo 'Mockifer: Editor: Cleaning up build folder...'
if [ -d "./build" ]; then
rm -rf "./build"
fi

echo "Mockifer: Editor: Build complete! Find 'MockiferEditor.app' in the root folder."
echo ''
echo '======================================================='
echo 'MOCKIFER: EDITOR BUILD FINISHED'
echo '======================================================='
echo ''