package io.github.marcelbraghetto.mockifer.example.android.cats

import com.google.gson.annotations.SerializedName
import io.github.marcelbraghetto.mockifer.example.android.cats.Cat

class Cats(@SerializedName("cats") val cats: Array<Cat>)
