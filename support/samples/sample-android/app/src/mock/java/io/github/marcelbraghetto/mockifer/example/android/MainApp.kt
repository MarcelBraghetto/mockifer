package io.github.marcelbraghetto.mockifer.example.android

import io.github.marcelbraghetto.mockifer.Mockifer
import io.github.marcelbraghetto.mockifer.example.android.main.BaseApplication

class MainApp : BaseApplication() {
    override fun onCreate() {
        super.onCreate()
        Mockifer.installOnPort(this, this.applicationContext, 8507)
    }
}
