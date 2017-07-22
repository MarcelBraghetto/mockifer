package mockifer.example.android.edit;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.gson.Gson;

import java.io.IOException;

import mockifer.example.android.R;
import mockifer.example.android.cats.Cat;
import mockifer.example.android.common.Requester;
import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.Response;

public class EditCatActivity extends AppCompatActivity {
    public static final String EXTRA_CAT = "cat";

    Button deleteButton;
    Button saveButton;
    TextView catIdTextView;
    EditText catNameEditText;
    EditText catAgeEditText;

    Cat cat;

    @Override public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_edit_cat);

        catIdTextView = (TextView) findViewById(R.id.catId);
        catNameEditText = (EditText) findViewById(R.id.catName);
        catAgeEditText = (EditText) findViewById(R.id.catAge);

        cat = (Cat) getIntent().getSerializableExtra(EXTRA_CAT);

        catIdTextView.setText(cat.id);
        catNameEditText.setText(cat.name);
        catAgeEditText.setText(String.valueOf(cat.age));

        deleteButton = (Button) findViewById(R.id.deleteButton);
        saveButton = (Button) findViewById(R.id.saveButton);

        deleteButton.setOnClickListener(new View.OnClickListener() {
            @Override public void onClick(View view) {
                deleteCat();
            }
        });

        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override public void onClick(View view) {
                saveCat();
            }
        });
    }

    void deleteCat() {
        Toast.makeText(this, "Deleting cat...", Toast.LENGTH_SHORT).show();

        Requester.doRequest("DELETE", "/cats/" + cat.id, null, new Callback() {
            @Override public void onFailure(Call call, IOException e) {

            }

            @Override public void onResponse(Call call, final Response response) throws IOException {
                runOnUiThread(new Runnable() {
                    @Override public void run() {
                        if (!response.isSuccessful()) {
                            Toast.makeText(EditCatActivity.this, "Error deleting cat", Toast.LENGTH_SHORT).show();
                        }
                        try { response.body().string(); } catch (IOException e) {}
                        finish();
                    }
                });
            }
        });
    }

    void saveCat() {
        Toast.makeText(this, "Saving cat...", Toast.LENGTH_SHORT).show();

        Cat catToSave = new Cat();
        catToSave.id = cat.id;
        catToSave.name = catNameEditText.getText().toString();
        catToSave.age = Integer.valueOf(catAgeEditText.getText().toString());
        catToSave.image = cat.image;

        Requester.doRequest("PUT", "/cats", new Gson().toJson(catToSave), new Callback() {
            @Override public void onFailure(Call call, IOException e) {

            }

            @Override public void onResponse(Call call, final Response response) throws IOException {
                runOnUiThread(new Runnable() {
                    @Override public void run() {
                        if (!response.isSuccessful()) {
                            Toast.makeText(EditCatActivity.this, "Error updating cat", Toast.LENGTH_SHORT).show();
                        }
                        try {
                            response.body().string();
                        } catch (IOException e) {
                        }
                        finish();
                    }
                });
            }
        });
    }
}
