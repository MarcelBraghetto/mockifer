package mockifer.example.android.common;

import okhttp3.Callback;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;

public class Requester {
    public static final String BASE_URL = "http://localhost:8503";
    private static OkHttpClient client = new OkHttpClient();

    private Requester() {

    }

    public static void doRequest(String method, String uri, String body, Callback callback) {
        RequestBody requestBody = null;

        if (body != null) {
            requestBody = RequestBody.create(MediaType.parse("utf-8"), body);
        }

        Request request = new Request.Builder()
                .url(BASE_URL + uri)
                .method(method, requestBody)
                .build();

        client.newCall(request).enqueue(callback);
    }
}
