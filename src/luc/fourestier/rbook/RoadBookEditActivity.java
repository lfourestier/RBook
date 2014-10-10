package luc.fourestier.rbook;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.v4.app.NavUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class RoadBookEditActivity extends Activity {
    private EditText titleEditText;
    private EditText descriptionEditText;
    private EditText locationEditText;
    private TextView distanceTextView;
    private ImageView bookImageView;
    private Button startButton;

    private RoadBook currentRoadBook = null;
	private BookManager theBookManager = null;

// Activity
    
    @Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_road_book_edit);
		setupActionBar();
		try {
			currentRoadBook = MainActivity.currentRoadBook;
			theBookManager = MainActivity.theBookManager;
			
	    	titleEditText = (EditText) findViewById(R.id.book_edit_name_text);
	    	descriptionEditText = (EditText) findViewById(R.id.book_edit_description_text);
	    	locationEditText = (EditText) findViewById(R.id.book_edit_location_text);
	    	distanceTextView = (TextView) findViewById(R.id.book_edit_distance_text);
	    	bookImageView = (ImageView) findViewById(R.id.book_edit_image);
	    	startButton = (Button) findViewById(R.id.book_edit_start_button);
	    	startButton.setOnClickListener(mStartListener);
	    	
	    	setTitle(currentRoadBook.getBookName());
	    	refreshRoadBookView(currentRoadBook);
		}
		catch (Exception e) {
			Log.e(TAG, "Cannot load: " + e.getMessage());
			toastMessage("Oups! Cannot load!");
		}
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
        case R.id.action_book_save:
        	try {
	        	setValueFromRoadBookView(currentRoadBook);
			    theBookManager.saveRoadBook(currentRoadBook);
	    	    Intent intent = new Intent(this, MainActivity.class);
				NavUtils.navigateUpTo(this, intent);
			} catch (Exception e) {
				Log.e(TAG, "Error while saving!" + e.getMessage());
				toastMessage("Oups! Cannot save!");
			}
            return true;
		}
		return super.onOptionsItemSelected(item);
	}

// Start button
	
	private OnClickListener mStartListener = new OnClickListener() {
        public void onClick(View v) {
        	try {
        		setValueFromRoadBookView(currentRoadBook);
        		
        	    Intent intent = new Intent(RoadBookEditActivity.this, RoadPointEditActivity.class);
        	    startActivity(intent);
			} 
        	catch (Exception e) {
				Log.e(TAG, "Cannot load point list: " + e.getMessage());
				toastMessage("Oups! Cannot load!");
			}
        }
    };
    
// Road book    
    
	private void refreshRoadBookView(RoadBook roadbook) {
		Float distance = 0.0f;
		titleEditText.setText(roadbook.getTitle());
		descriptionEditText.setText(roadbook.getDescription());
		locationEditText.setText(roadbook.getLocation());
		
		try {
			distance = roadbook.getTotalDistance();
		} catch (IndexOutOfBoundsException e) {
			distance = 0.0f;
		}
	    distanceTextView.setText(String.format("%.2f", distance));
	    
 	    String path = roadbook.getImage();
 	    if (!path.isEmpty()) {
 	        Bitmap bitmap = BitmapFactory.decodeFile(path);
 	        bookImageView.setImageBitmap(bitmap);
 	    }

	}
	
	private void setValueFromRoadBookView(RoadBook roadbook) {
		roadbook.setTitle(titleEditText.getText().toString());
		roadbook.setDescription(descriptionEditText.getText().toString());
		roadbook.setLocation(locationEditText.getText().toString());
	}
		
// Toast
    private static final String TAG = RoadBookEditActivity.class.getSimpleName();
    
    private void toastMessage(String message) {
	    Toast toast = Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT);
	    if (toast != null) {
	    	toast.show();		
	    }
    }
}
