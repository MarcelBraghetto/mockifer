package mockifer.example.android.cats

import com.google.gson.annotations.SerializedName

class Cats(@SerializedName("cats") val cats: Array<Cat>)
