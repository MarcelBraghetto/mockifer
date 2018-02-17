package mockifer.example

import android.net.Uri
import android.support.test.InstrumentationRegistry
import android.support.test.rule.ActivityTestRule
import android.support.v7.app.AppCompatActivity
import mockifer.Mockifer
import mockifer.example.android.BuildConfig
import mockifer.example.android.main.BaseApplication

class MockiferTestRule<T : AppCompatActivity>(activityClass: Class<T>) : ActivityTestRule<T>(activityClass, true, false) {
    private companion object {
        var mockiferInitialised = false
    }

    init {
        if (!mockiferInitialised) {
            mockiferInitialised = true

            val uri = Uri.parse(BuildConfig.SERVER_BASE_URL)
            val port = uri.port

            Mockifer.installOnPort(BaseApplication.get(), InstrumentationRegistry.getInstrumentation().context, port)
            Mockifer.setCommandUrl(uri.host, port)
        }
    }
}
