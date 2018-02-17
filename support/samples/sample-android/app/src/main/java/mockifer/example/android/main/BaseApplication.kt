package mockifer.example.android.main

import android.annotation.SuppressLint
import android.app.Application

abstract class BaseApplication : Application() {
    companion object {
        @SuppressLint("StaticFieldLeak") private lateinit var instance: Application

        fun get(): Application {
            return instance
        }
    }

    override fun onCreate() {
        super.onCreate()
        instance = this
    }
}