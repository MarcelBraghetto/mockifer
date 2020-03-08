package io.github.marcelbraghetto.mockifer.example.android.main

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager.VERTICAL
import androidx.recyclerview.widget.RecyclerView
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.View.GONE
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import com.google.gson.Gson
import com.squareup.picasso.Picasso
import io.github.marcelbraghetto.mockifer.example.android.BuildConfig
import io.github.marcelbraghetto.mockifer.example.android.R
import io.github.marcelbraghetto.mockifer.example.android.cats.Cat
import io.github.marcelbraghetto.mockifer.example.android.cats.Cats
import io.github.marcelbraghetto.mockifer.example.android.common.Requester
import kotlinx.android.synthetic.main.activity_main.*
import io.github.marcelbraghetto.mockifer.example.android.edit.EditCatActivity
import io.github.marcelbraghetto.mockifer.example.android.MockiferUtil
import okhttp3.Call
import okhttp3.Callback
import okhttp3.Response
import java.io.IOException

class MainActivity : AppCompatActivity() {
    private var cats = arrayOf<Cat>()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        recyclerView.run {
            layoutManager = LinearLayoutManager(this@MainActivity, VERTICAL, false)
            adapter = CatsAdapter()
        }

        if (BuildConfig.IS_MOCK_APP) {
            resetButton.setOnClickListener { resetServerData() }

            mockErrorButton.setOnClickListener {
                // Push a mock into the collection of routes that are evaluated for requests.
                MockiferUtil.pushMock(mockId = "mocks.cats.getcats.error")
                loadCats()
            }
        } else {
            resetButton.visibility = GONE
            mockErrorButton.visibility = GONE
        }
    }

    override fun onResume() {
        super.onResume()
        loadCats()
    }

    private fun resetServerData() {
        MockiferUtil.reset()
        loadCats()
    }

    private fun loadCats() {
        Requester.doRequest("GET", "/cats", null, object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                Log.d("BLAH", "Load cats failed: ${e.localizedMessage}")
            }

            @Throws(IOException::class)
            override fun onResponse(call: Call, response: Response) {
                val statusCode = response.code()
                val responseBody = response.body()!!.string()

                if (!response.isSuccessful) {
                    runOnUiThread { showErrorDialog("Status code: " + statusCode + "\n\n" + responseBody) }
                    return
                }

                cats = Gson().fromJson(responseBody, Cats::class.java).cats
                runOnUiThread { recyclerView.adapter?.notifyDataSetChanged() }
            }
        })
    }

    private fun showErrorDialog(message: String) {
        AlertDialog.Builder(this)
                .setTitle("Network Error")
                .setMessage(message)
                .setPositiveButton("OK", null)
                .create()
                .show()
    }

    private fun catSelected(position: Int) {
        startActivity(Intent(this, EditCatActivity::class.java).apply {
            putExtra(EditCatActivity.EXTRA_CAT, cats[position])
        })
    }

    private inner class CatsAdapter : RecyclerView.Adapter<CatsAdapter.ViewHolder>() {
        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
            return ViewHolder(LayoutInflater.from(parent.context).inflate(R.layout.cat_list_item, parent, false))
        }

        override fun onBindViewHolder(holder: ViewHolder, position: Int) {
            holder.bind(cats[position])
        }

        override fun getItemCount(): Int {
            return cats.size
        }

        internal inner class ViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
            private val imageView = itemView.findViewById<ImageView>(R.id.catImageView)
            private val idTextView = itemView.findViewById<TextView>(R.id.catIdTextView)
            private val nameTextView = itemView.findViewById<TextView>(R.id.catNameTextView)
            private val ageTextView = itemView.findViewById<TextView>(R.id.catAgeTextView)

            init {
                itemView.setOnClickListener { catSelected(adapterPosition) }
            }

            fun bind(cat: Cat) {
                Picasso.with(itemView.context)
                        .load(BuildConfig.SERVER_BASE_URL + cat.image)
                        .into(imageView)

                idTextView.text = "Id: ${cat.id}"
                nameTextView.text = "Name: ${cat.name}"
                ageTextView.text = "Age: ${cat.age}"
            }
        }
    }
}
