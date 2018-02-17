package mockifer.example.android.cats

import com.google.gson.annotations.SerializedName
import java.io.Serializable

class Cat(@SerializedName("id") var id: String,
          @SerializedName("name") var name: String,
          @SerializedName("age") var age: Int,
          @SerializedName("image") var image: String
) : Serializable
