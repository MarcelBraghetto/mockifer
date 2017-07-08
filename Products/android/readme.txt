After building there will be a ‘mockifer.aar’ package here and a ‘mockifer-js’ folder.

The .aar library only needs to be integrated into your Android app once (unless the core code has been changed).

The ‘mockifer-js’ folder can then be freely updated in the ‘assets’ folder of your project as changes are made to the javascript engine.

The default port will be 8503 but can be changed when ‘installing’ the library in your main application class.

Open the Products/sample-android/ project for an example of how to integrate the framework.