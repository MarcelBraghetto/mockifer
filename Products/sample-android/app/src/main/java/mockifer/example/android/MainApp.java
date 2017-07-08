package mockifer.example.android;

import android.app.Application;

import mockifer.Mockifer;

public class MainApp extends Application {
    @Override public void onCreate() {
        super.onCreate();
        Mockifer.install(this);
    }
}
