package mockifer.example.android

import mockifer.Mockifer
import mockifer.example.android.main.BaseApplication

class MainApp : BaseApplication() {
    override fun onCreate() {
        super.onCreate()
        Mockifer.installOnPort(this, this.applicationContext, 8507)
    }
}
