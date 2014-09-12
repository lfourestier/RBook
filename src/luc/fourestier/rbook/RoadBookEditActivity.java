package luc.fourestier.rbook;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.NavUtils;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class RoadBookEditActivity extends Activity {
    private EditText titleEditText;
    private EditText descriptionEditText;
    private EditText locationEditText;
    private Button startButton;

    private RoadBook currentRoadBook = null;
    private String currentBookName;

// Activity
    
    @Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_road_book_edit);
		setupActionBar();

		currentRoadBook = MainActivity.currentRoadBook;
		currentBookName = MainActivity.currentBookName;
		
    	titleEditText = (EditText) findViewById(R.id.book_edit_name_text);
    	descriptionEditText = (EditText) findViewById(R.id.book_edit_description_text);
    	locationEditText = (EditText) findViewById(R.id.book_edit_location_text);
    	startButton = (Button) findViewById(R.id.book_edit_start_button);
    	startButton.setOnClickListener(mStartListener);
    	
    	setTitle(currentBookName);
	}

// Action bar
    
    private void setupActionBar() {

		getActionBar().setDisplayHomeAsUpEnabled(true);

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.road_book_edit, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case android.R.id.home:
			NavUtils.navigateUpFromSameTask(this);
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

// Start button
	
	private OnClickListener mStartListener = new OnClickListener() {
        public void onClick(View v) {
        	try {
        		currentRoadBook.setTitle(titleEditText.getText().toString());
        		currentRoadBook.setDescription(descriptionEditText.getText().toString());
        		currentRoadBook.setLocation(locationEditText.getText().toString());
        		
        	    Intent intent = new Intent(RoadBookEditActivity.this, RoadPointEditActivity.class);
        	    startActivity(intent);
			} 
        	catch (Exception e) {
				// TODO: handle exception
			}
        }
    };
    
// Toast
    
    private void toastMessage(String message) {
	    Toast toast = Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT);
	    if (toast != null) {
	    	toast.show();		
	    }
    }
}
