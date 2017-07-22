package mockifer.example.android.main;

import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.google.gson.Gson;
import com.squareup.picasso.Picasso;

import java.io.IOException;

import mockifer.Mockifer;
import mockifer.example.android.R;
import mockifer.example.android.cats.Cat;
import mockifer.example.android.cats.Cats;
import mockifer.example.android.common.Requester;
import mockifer.example.android.edit.EditCatActivity;
import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.Response;

import static android.support.v7.widget.LinearLayoutManager.VERTICAL;

public class MainActivity extends AppCompatActivity {
    Button resetButton;
    Button mockErrorButton;
    RecyclerView recyclerView;

    Cat[] cats = new Cat[0];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        resetButton = (Button) findViewById(R.id.resetButton);
        mockErrorButton= (Button) findViewById(R.id.mockErrorButton);

        recyclerView = (RecyclerView) findViewById(R.id.recyclerView);
        recyclerView.setLayoutManager(new LinearLayoutManager(this, VERTICAL, false));
        recyclerView.setAdapter(new CatsAdapter());

        resetButton.setOnClickListener(new View.OnClickListener() {
            @Override public void onClick(View view) {
                resetServerData();
            }
        });

        mockErrorButton.setOnClickListener(new View.OnClickListener() {
            @Override public void onClick(View view) {
                // Push a mock into the collection of routes that are evaluated for requests.
                Mockifer.pushMock("mocks.cats.getcats.error");
                loadCats();
            }
        });
    }

    @Override protected void onResume() {
        super.onResume();
        loadCats();
    }

    void resetServerData() {
        Mockifer.reset();
        loadCats();
    }

    void loadCats() {
        Requester.doRequest("GET", "/cats", null, new Callback() {
            @Override public void onFailure(@NonNull Call call, @NonNull IOException e) {
            }

            @Override public void onResponse(@NonNull Call call, @NonNull Response response) throws IOException {
                final int statusCode = response.code();
                final String responseBody = response.body().string();

                if (!response.isSuccessful()) {
                    runOnUiThread(new Runnable() {
                        @Override public void run() {
                            showErrorDialog("Status code: " + statusCode + "\n\n" + responseBody);
                        }
                    });
                    return;
                }

                cats = new Gson().fromJson(responseBody, Cats.class).cats;
                runOnUiThread(new Runnable() {
                    @Override public void run() {
                        recyclerView.getAdapter().notifyDataSetChanged();
                    }
                });
            }
        });
    }

    private void showErrorDialog(String message) {
        new AlertDialog.Builder(this)
                .setTitle("Network Error")
                .setMessage(message)
                .setPositiveButton("OK", null)
                .create()
                .show();
    }

    private void catSelected(int position) {
        Intent intent = new Intent(this, EditCatActivity.class);
        intent.putExtra(EditCatActivity.EXTRA_CAT, cats[position]);
        startActivity(intent);
    }

    private class CatsAdapter extends RecyclerView.Adapter<CatsAdapter.ViewHolder> {
        @Override public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            return new ViewHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.cat_list_item, parent, false));
        }

        @Override public void onBindViewHolder(ViewHolder holder, int position) {
            holder.bind(cats[position]);
        }

        @Override public int getItemCount() {
            return cats.length;
        }

        class ViewHolder extends RecyclerView.ViewHolder {
            private ImageView catImageView;
            private TextView catIdTextView;
            private TextView catNameTextView;
            private TextView catAgeTextView;

            ViewHolder(View itemView) {
                super(itemView);
                catImageView = (ImageView) itemView.findViewById(R.id.catImage);
                catIdTextView = (TextView) itemView.findViewById(R.id.catId);
                catNameTextView = (TextView) itemView.findViewById(R.id.catName);
                catAgeTextView = (TextView) itemView.findViewById(R.id.catAge);

                itemView.setOnClickListener(new View.OnClickListener() {
                    @Override public void onClick(View view) {
                        catSelected(getAdapterPosition());
                    }
                });
            }

            void bind(Cat cat) {
                Picasso.with(itemView.getContext()).load(Requester.BASE_URL + cat.image).into(catImageView);
                catIdTextView.setText("Id:     " + cat.id);
                catNameTextView.setText("Name: " + cat.name);
                catAgeTextView.setText("Age:   " + cat.age);
            }
        }
    }
}
