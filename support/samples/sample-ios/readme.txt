Before running the iOS sample for the first time you will need to:

1. Copy the 'mockifer-js' folder from the 'products/content/ios' folder and paste it into the 'support/samples/sample-ios/MockiferSample' folder, overwriting any existing 'mockifer-js' folder.
2. Copy 'products/libs/ios/Mockifer.Framework' into the 'support/samples/sample-ios' folder you just created so the iOS sample app can reference it.

Once you do these steps you should be able to successfully open the sample and run the iOS sample app.

The normal use cases are that the 'Mockifer.Framework' would pretty much never change unless you wanted to change the core engine, but the 'mockifer-js' folder would change every time you compile the content (due to changing routes or adding Kotlin JS code). It's up to you to decide how to manage content changes for your own projects.
