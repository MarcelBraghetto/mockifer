set -e

echo ''
echo '======================================================='
echo 'MOCKIFER: CONSOLE BUILD STARTING'
echo '======================================================='
echo ''

PRODUCT_PATH="./../../../mockifer"

echo 'Mockifer: Console: Cleaning build folder...'
if [ -d "./build" ]; then
rm -rf "./build"
fi

echo 'Mockifer: Console: Building target...'
xcodebuild -target "mockifer"  > /dev/null

echo 'Mockifer: Console: Removing existing app...'
if [ -d "${PRODUCT_PATH}" ]; then
rm -rf "${PRODUCT_PATH}"
fi

echo 'Mockifer: Console: Copying app to output path...'
cp -r "./build/Release/mockifer" "${PRODUCT_PATH}"

echo 'Mockifer: Console: Cleaning up build folder...'
if [ -d "./build" ]; then
rm -rf "./build"
fi

echo "Mockifer: Console: Build complete! Find 'mockifer' in the root folder."
echo ''
echo '======================================================='
echo 'MOCKIFER: CONSOLE BUILD FINISHED'
echo '======================================================='
echo ''