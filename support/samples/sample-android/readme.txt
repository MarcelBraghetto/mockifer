Before running the Android sample for the first time you will need to:

1. Copy the 'mockifer-js' folder from the 'products/content/android' folder and paste it into the 'support/samples/sample-android/app/mockifer' folder, overwriting any existing 'mockifer-js' folder.
2. Create a new folder named 'libs' in the 'support/samples/sample-android/app' folder if it doesn't exist already.
3. Copy the 'products/libs/android/mockifer.aar' into the 'libs' folder you just created so the Android sample app can reference it.

Once you do these steps you should be able to successfully open the sample and run the Android sample app.

The normal use cases are that the 'mockifer.aar' would pretty much never change unless you wanted to change the core engine, but the 'mockifer-js' folder would change every time you compile the content (due to changing routes or adding Kotlin JS code). It's up to you to decide how to manage content changes for your own projects.
