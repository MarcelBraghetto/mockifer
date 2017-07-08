set -e

echo ''
echo '======================================================='
echo 'MOCKIFER: iOS FRAMEWORK BUILD STARTING'
echo '======================================================='
echo ''

FRAMEWORK_NAME="Mockifer"
PRODUCT_PATH="./../../Products/ios/${FRAMEWORK_NAME}.framework"
SAMPLE_IOS_APP_PRODUCT_PATH="./../../Products/sample-ios/${FRAMEWORK_NAME}.framework"

echo 'Mockifer: iOS Framework: Cleaning build folder...'
if [ -d "./build" ]; then
rm -rf "./build"
fi

echo 'Mockifer: iOS Framework: Building debug iphonesimulator target...'
xcodebuild -target "${FRAMEWORK_NAME}" -configuration Debug -arch x86_64 -arch i386 only_active_arch=no defines_module=yes -sdk "iphonesimulator" > /dev/null

echo 'Mockifer: iOS Framework: Building debug iphoneos target...'
xcodebuild -target "${FRAMEWORK_NAME}" -configuration Debug -arch arm64 -arch armv7 -arch armv7s only_active_arch=no defines_module=yes -sdk "iphoneos" > /dev/null

echo 'Mockifer: iOS Framework: Removing existing framework...'
if [ -d "${PRODUCT_PATH}" ]; then
rm -rf "${PRODUCT_PATH}"
fi

echo 'Mockifer: iOS Framework: Copying iphoneos target to framework...'
cp -r "./build/Debug-iphoneos/${FRAMEWORK_NAME}.framework" "${PRODUCT_PATH}"

echo 'Mockifer: iOS Framework: Creating fat library with iphoneos and iphonesimulator executables...'
lipo -create -output "${PRODUCT_PATH}/${FRAMEWORK_NAME}" "./build/Debug-iphonesimulator/${FRAMEWORK_NAME}.framework/${FRAMEWORK_NAME}" "./build/Debug-iphoneos/${FRAMEWORK_NAME}.framework/${FRAMEWORK_NAME}"

echo 'Mockifer: iOS Framework: Cleaning up build folder...'
if [ -d "./build" ]; then
rm -rf "./build"
fi

echo 'Mockifer: iOS Framework: Updating Mockifer.framework in the Sample iOS app...'

# Delete existing framework from sample ios app
if [ -d "${SAMPLE_IOS_APP_PRODUCT_PATH}" ]; then
rm -rf "${SAMPLE_IOS_APP_PRODUCT_PATH}"
fi

# Copy newly created framework into sample ios app
cp -r "${PRODUCT_PATH}" "${SAMPLE_IOS_APP_PRODUCT_PATH}"

echo "Mockifer: iOS Framework: Build complete! Find '${FRAMEWORK_NAME}.framework' in the Products/ios folder. See the 'example-ios' project to find out how to integrate the framework."
echo ''
echo '======================================================='
echo 'MOCKIFER: iOS FRAMEWORK BUILD FINISHED'
echo '======================================================='
echo ''