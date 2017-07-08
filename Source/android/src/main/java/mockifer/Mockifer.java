/*
 * =================================================
 * Mockifer License
 * =================================================
 *
 * MIT License
 *
 * Copyright (c) 2017 Marcel Braghetto
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

package mockifer;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.NonNull;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.concurrent.atomic.AtomicInteger;

@SuppressWarnings({"unused", "WeakerAccess"})
public final class Mockifer {
    private static String CONTENT_PATH = "";
    private static int SERVER_PORT = 8503;

    private Mockifer() {
    }

    static {
        System.loadLibrary("mockifer");
    }

    /**
     * Install Mockifer with the default settings - access via http://localhost:8700
     *
     * @param application to associate with Mockifer.
     */
    public static void install(@NonNull Application application) {
        install(application, SERVER_PORT);
    }

    /**
     * Install Mockifer with the given port number to listen on - access via http://localhost:YOUR_PORT_NUMBER
     *
     * @param application to associate with Mockifer.
     * @param port        to listen on.
     */
    public static void install(@NonNull Application application, int port) {
        CONTENT_PATH = application.getFilesDir().getAbsolutePath() + "/mockifer-js";
        SERVER_PORT = port;

        try {
            File mockiferDirectory = new File(CONTENT_PATH);
            if (mockiferDirectory.exists()) {
                delete(mockiferDirectory);
            }

            copy(application, "mockifer-js", "mockifer-js");
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }

        application.registerActivityLifecycleCallbacks(new MockiferLifecycle());
    }

    /**
     * Reset the Mockifer server which will restore it to a clean state.
     */
    public static void reset() {
        nativeResetServer();
    }

    /**
     * Set the global response delay to apply to all requests to simulate network timings.
     * The 'reset' method will also clear this value.
     *
     * @param responseDelayMillis to wait before responding to each request.
     */
    public static void setGlobalResponseDelay(int responseDelayMillis) {
        nativeSetGlobalResponseDelay(Math.abs(responseDelayMillis));
    }

    /**
     * Clear any active mocks that have been pushed - other server state will remain intact.
     */
    public static void clearActiveMocks() {
        nativeClearActiveMocks();
    }

    /**
     * Push a collection of mock route identifiers to be evaluated when matching request routes.
     *
     * Each added mock route id will be executed once by default before being consumed.
     *
     * @param mockRouteIds to push to evaluate for incoming requests.
     */
    public static void pushMocks(@NonNull String... mockRouteIds) {
        for (String mockRouteId : mockRouteIds) {
            pushMock(mockRouteId, 1);
        }
    }

    /**
     * Push a single mock route id to be evaluated when matching request routes. The mock route id
     * will be executed once by default before being consumed.
     *
     * @param mockRouteId to push to evaluate for incoming requests.
     */
    public static void pushMock(@NonNull String mockRouteId) {
        pushMock(mockRouteId, 1);
    }

    /**
     * Push a single mock id to be evaluated when matching request routes along with how many times
     * it can be executed before being consumed.
     *
     * @param mockRouteId to push to evaluate for incoming requests.
     * @param times       to execute the mock before it is considered to have been consumed.
     */
    public static void pushMock(@NonNull String mockRouteId, int times) {
        nativePushMock(mockRouteId, times);
    }

    //region Native C/C++ methods
    private static native void nativeStartServer(String contentPath, String port);

    private static native void nativeStopServer();

    private static native void nativeResetServer();

    private static native void nativeSetGlobalResponseDelay(int delay);

    private static native void nativePushMock(String mockRouteId, int times);

    private static native void nativeClearActiveMocks();
    //endregion

    //region Lifecycle management
    private static class MockiferLifecycle implements Application.ActivityLifecycleCallbacks {
        private AtomicInteger activityCount = new AtomicInteger(0);

        private void killServerIfNeeded() {
            new Handler().postDelayed(new Runnable() {
                @Override public void run() {
                    int count = activityCount.get();
                    if (count <= 0) {
                        nativeStopServer();
                    }
                }
            }, 1000L * 60L * 5L);
        }

        @Override public void onActivityCreated(Activity activity, Bundle bundle) {
            int count = activityCount.incrementAndGet();

            if (count <= 0) {
                return;
            }

            if (CONTENT_PATH == null || CONTENT_PATH.length() == 0) {
                return;
            }

            nativeStartServer(CONTENT_PATH, String.valueOf(SERVER_PORT));
        }

        @Override public void onActivityStarted(Activity activity) {
        }

        @Override public void onActivityResumed(Activity activity) {

        }

        @Override public void onActivityPaused(Activity activity) {

        }

        @Override public void onActivityStopped(Activity activity) {
        }

        @Override public void onActivitySaveInstanceState(Activity activity, Bundle bundle) {

        }

        @Override public void onActivityDestroyed(Activity activity) {
            activityCount.decrementAndGet();
            killServerIfNeeded();
        }
    }
    //endregion

    //region File management
    private static void delete(File target) {
        File[] fileList = target.listFiles();
        if (fileList != null) {
            for (File file : fileList) {
                delete(file);
            }
        }
        target.delete();
    }

    private static String copy(Context context, String assetPath, String destinationPath) throws IOException {
        File filesDirectory = context.getFilesDir();
        String targetPath = filesDirectory + addLeadingSlash(destinationPath);
        File targetDirectory = new File(targetPath);

        createDir(targetDirectory);

        AssetManager assetManager = context.getApplicationContext().getAssets();
        String[] fileNameList = assetManager.list(assetPath);

        for (String fileName : fileNameList) {
            String filePath = addTrailingSlash(assetPath) + fileName;
            String fileChildren[] = assetManager.list(filePath);

            if (fileChildren.length == 0) {
                String targetFilePath = addTrailingSlash(targetPath) + fileName;
                copyAssetFile(context, filePath, targetFilePath);
            } else {
                copy(context, filePath, addTrailingSlash(destinationPath) + fileName);
            }
        }

        return targetPath;
    }


    private static void copyAssetFile(Context context, String assetFilePath, String destinationFilePath)
            throws IOException {
        InputStream in = context.getApplicationContext().getAssets().open(assetFilePath);
        OutputStream out = new FileOutputStream(destinationFilePath);

        byte[] buf = new byte[1024];
        int len;
        while ((len = in.read(buf)) > 0) {
            out.write(buf, 0, len);
        }
        in.close();
        out.close();
    }

    private static String addTrailingSlash(String path) {
        if (path.charAt(path.length() - 1) != '/') {
            path += "/";
        }
        return path;
    }

    private static String addLeadingSlash(String path) {
        if (path.charAt(0) != '/') {
            path = "/" + path;
        }
        return path;
    }

    private static void createDir(File directory) throws IOException {
        if (directory.exists()) {
            if (!directory.isDirectory()) {
                throw new IOException("A file of the same name already exists.");
            }
        } else {
            directory.mkdirs();
            if (!directory.isDirectory()) {
                throw new IOException("Unable to create directory");
            }
        }
    }
    //endregion
}
