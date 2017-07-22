package mockifer.example.android.cats;

import com.google.gson.annotations.SerializedName;

import java.io.Serializable;

public class Cat implements Serializable {
    @SerializedName("id") public String id;
    @SerializedName("name") public String name;
    @SerializedName("age") public int age;
    @SerializedName("image") public String image;
}
