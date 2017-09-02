After building there will be a ‘mockifer.aar’ package here and a ‘mockifer-js’ folder.

The .aar library only needs to be integrated into your Android app once (unless the core code has been changed).

The ‘mockifer-js’ folder can then be freely updated in the ‘assets’ folder of your project as changes are made to the javascript engine.

The default port will be 8503 but can be changed when ‘installing’ the library in your main application class.

Open the Products/sample-android/ project for an example of how to integrate the framework.

IMPORTANT: For Android, the ‘mockifer-js’ content needs to be archived into a .zip file during compilation to be included in the installed assets. This is because to access the content in a device, it has to be first copied into internal storage. Unfortunately the Android Asset Manager is INCREDIBLY slow at copy operations from assets -> internal. Instead, the .zip file is copied to internal storage then inflated directly which is a very fast operation in comparison to copying each file individually from the assets folder.

Make sure that in your Android Gradle file you include the asset zipping task and pre build dependency step. There is no need to include the resulting ‘mockifer-js.zip’ archive in source control - you should add an ignore rule for it.

See the sample Android app for an example, but typically the Gradle task would look something like:

// Zip up the ‘mockifer-js’ assets folder into a ‘mockifer-js.zip’ file.
task zipMockiferAssets(type: Zip) {
    from fileTree("${projectDir}/src/main/assets/mockifer-js")
    destinationDir new File("${projectDir}/src/main/assets/mockifer-js/")
    archiveName "mockifer-js.zip"
    exclude "mockifer-js.zip"
}

// Cause the pre build step to perform the asset zipping automatically.
preBuild.dependsOn zipMockiferAssets
