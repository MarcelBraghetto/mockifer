#!/bin/bash

# Important! Before running this script you will probably need to have installed all the required
# Xcode and Android SDK components. Also the Mockifer Editor will need to have a signing profile
# associated with it. It is recommended that you:
# - Import the Android library project into Android Studio and assert you have all the SDK and
#   Gradle dependencies required to build it.
# - Open the Mockifer Editor Xcode project and pick a signing profile.
# Technically you can build each thing directly in Android Studio/Xcode but this script should
# automate all of them in one hit.

# Alias the 'pushd' command and have it send its output to the abyss ...
pushd() {
	command pushd "$@" > /dev/null
}

# Alias the 'popd' command and have it send its output to the abyss ...
popd() {
	command popd "$@" > /dev/null
}

# If there are any products you don't want to build, mark them as false.
BUILD_CONTENT=true
BUILD_ANDROID_LIBRARY=true
BUILD_IOS_FRAMEWORK=true
BUILD_CONSOLE_APPLICATION=true
BUILD_MOCKIFER_EDITOR=true

echo ".:: Building Mockifer products ::."
echo ""

if [ "$BUILD_CONTENT" = true ] ; then
	echo ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ."
	echo "Building Mockifer content ..."
	pushd content
		./gradlew mockiferBuildContent
	popd
	echo "Mockifer content build complete - content can be found in the 'products/content' directory"
	echo ""
else
	echo "Skipping Mockifer content build ..."
	echo ""
fi

if [ "$BUILD_ANDROID_LIBRARY" = true ] ; then
	echo ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ."
	echo "Building Mockifer Android AAR Library ..."
	pushd support/source/android
		./build.sh
	popd
	echo "Mockifer Android AAR Library build complete - 'mockifer.aar' can be found in the 'products/libs/android' directory"
	echo ""
else
	echo "Skipping Mockifer Android AAR Library build ..."
	echo ""
fi

if [ "$BUILD_IOS_FRAMEWORK" = true ] ; then
	echo ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ."
	echo "Building Mockifer iOS Framework ..."
	pushd support/source/ios
		./build.sh
	popd
	echo "Mockifer iOS Framework build complete - 'Mockifer.Framework' can be found in the 'products/libs/ios' directory"
	echo ""
else
	echo "Skipping Mockifer iOS Framework build ..."
	echo ""
fi

if [ "$BUILD_CONSOLE_APPLICATION" = true ] ; then
	echo ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ."
	echo "Building Mockifer Console Application ..."

    if [ -e "mockifer" ]; then
        rm mockifer
    fi

	pushd support/source/console
		./build.sh
	popd
	
	echo "Mockifer Console Application build complete - 'mockifer' executable can be found in this directory"
	echo ""
else
	echo "Skipping Console Application build ..."
	echo ""
fi

if [ "$BUILD_MOCKIFER_EDITOR" = true ] ; then
	echo ". . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ."
	echo "Building Mockifer Editor Application ..."

    if [ -d "MockiferEditor.app" ]; then
        rm -rf MockiferEditor.app
    fi

	pushd support/source/editor
		./build.sh
	popd

	echo "Mockifer Editor Application build complete - 'MockiferEditor.app' executable can be found in this directory"
	echo ""
else
	echo "Skipping Editor Application build ..."
	echo ""
fi
