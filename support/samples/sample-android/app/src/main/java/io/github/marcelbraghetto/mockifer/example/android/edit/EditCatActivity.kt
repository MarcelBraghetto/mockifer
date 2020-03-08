package io.github.marcelbraghetto.mockifer.example.android.edit

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import android.widget.Toast
import com.google.gson.Gson
import io.github.marcelbraghetto.mockifer.example.android.R
import io.github.marcelbraghetto.mockifer.example.android.cats.Cat
import io.github.marcelbraghetto.mockifer.example.android.common.Requester
import kotlinx.android.synthetic.main.activity_edit_cat.*
import okhttp3.Call
import okhttp3.Callback
import okhttp3.Response
import java.io.IOException

class EditCatActivity : AppCompatActivity() {
    private lateinit var cat: Cat

    public override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_edit_cat)

        cat = intent.getSerializableExtra(EXTRA_CAT) as Cat

        catIdTextView.text = cat.id
        catNameEditText.setText(cat.name)
        catAgeEditText.setText(cat.age.toString())

        deleteButton.setOnClickListener { deleteCat() }
        saveButton.setOnClickListener { saveCat() }
    }

    private fun deleteCat() {
        Toast.makeText(this, "Deleting cat...", Toast.LENGTH_SHORT).show()

        Requester.doRequest("DELETE", "/cats/" + cat.id, null, object : Callback {
            override fun onFailure(call: Call, e: IOException) {

            }

            @Throws(IOException::class)
            override fun onResponse(call: Call, response: Response) {
                runOnUiThread {
                    if (!response.isSuccessful) {
                        Toast.makeText(this@EditCatActivity, "Error deleting cat", Toast.LENGTH_SHORT).show()
                    }
                    try {
                        response.body()!!.string()
                    } catch (e: IOException) {
                    }

                    finish()
                }
            }
        })
    }

    private fun saveCat() {
        Toast.makeText(this, "Saving cat...", Toast.LENGTH_SHORT).show()

        val catToSave = Cat(
                id = cat.id,
                name = catNameEditText.text.toString(),
                age = Integer.valueOf(catAgeEditText.text.toString()),
                image = cat.image
        )

        Requester.doRequest("PUT", "/cats", Gson().toJson(catToSave), object : Callback {
            override fun onFailure(call: Call, e: IOException) {

            }

            @Throws(IOException::class)
            override fun onResponse(call: Call, response: Response) {
                runOnUiThread {
                    if (!response.isSuccessful) {
                        Toast.makeText(this@EditCatActivity, "Error updating cat", Toast.LENGTH_SHORT).show()
                    }
                    try {
                        response.body()!!.string()
                    } catch (e: IOException) {
                    }

                    finish()
                }
            }
        })
    }

    companion object {
        const val EXTRA_CAT = "cat"
    }
}
