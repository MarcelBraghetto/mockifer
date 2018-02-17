package mockifer.example.android.common

import mockifer.example.android.BuildConfig
import okhttp3.*

object Requester {
    private val client by lazy { OkHttpClient() }

    fun doRequest(method: String, uri: String, body: String?, callback: Callback) {
        val requestBody = body?.let {
            RequestBody.create(MediaType.parse("utf-8"), it)
        }

        val request = Request.Builder()
                .url(BuildConfig.SERVER_BASE_URL + uri)
                .method(method, requestBody)
                .build()

        client.newCall(request).enqueue(callback)
    }
}
