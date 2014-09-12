package luc.fourestier.rbook;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.NavUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class RoadBookActivity extends Activity {
    private TextView titleTextView;
    private TextView descriptionTextView;
    private TextView locationTextView;
    private TextView distanceTextView;
    private Button startButton;

	private BookManager theBookManager = null;
    private RoadBook currentRoadBook = null;

// Activity
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_road_book);
		setupActionBar();
		
		try {
			theBookManager = MainActivity.theBookManager;
			currentRoadBook = MainActivity.currentRoadBook;
			
	    	titleTextView = (TextView) findViewById(R.id.book_name_text);
	    	descriptionTextView = (TextView) findViewById(R.id.book_description_text);
	    	locationTextView = (TextView) findViewById(R.id.book_location_text);
	    	distanceTextView = (TextView) findViewById(R.id.book_distance_text);
	    	startButton = (Button) findViewById(R.id.book_start_button);
	    	startButton.setOnClickListener(mStartListener);
	    	
	    	setTitle(currentRoadBook.getBookName());
	    	refreshRoadBookView(currentRoadBook);
		}
    	catch (Exception e) {
			Log.e("RoadBookActivity", "Error while loading book: " + e.getMessage());
			toastMessage("Error while loading book!");
		}
	}

// Action bar
	
	private void setupActionBar() {

		getActionBar().setDisplayHomeAsUpEnabled(true);

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.road_book, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case android.R.id.home:
			NavUtils.navigateUpFromSameTask(this);
			return true;
		case R.id.action_delete:
        	try {
				theBookManager.deleteRoadBook(currentRoadBook.getBookName());
				theBookManager.releaseRoadBook(currentRoadBook);
				MainActivity.currentRoadBook = currentRoadBook = null;
				NavUtils.navigateUpFromSameTask(this);
        	}
        	catch (Exception e) {
				Log.e("RoadBookActivity", "Cannot delete: " + e.getMessage());
				toastMessage("Oups! Cannot delete!");
			}
			return true;
		case R.id.action_edit:
        	try {
				Intent intent = new Intent(this, RoadBookEditActivity.class);
				startActivity(intent);
        	}
        	catch (Exception e) {
				Log.e("RoadBookActivity", "Cannot edit: " + e.getMessage());
				toastMessage("Oups! Cannot edit!");
			}
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

// Buttons
	
    private OnClickListener mStartListener = new OnClickListener() {
        public void onClick(View v) {
        	try {
        	    Intent intent = new Intent(RoadBookActivity.this, RoadPointActivity.class);
        	    startActivity(intent);
			} 
        	catch (Exception e) {
				Log.e("RoadBookActivity", "Cannot load point list: " + e.getMessage());
				toastMessage("Cannot load point list!");
			}
        }
    };
    
 // Road book    
    
 	private void refreshRoadBookView(RoadBook roadbook) {
 		Float distance = 0.0f;
 		titleTextView.setText(roadbook.getTitle());
 		descriptionTextView.setText(roadbook.getDescription());
 		locationTextView.setText(roadbook.getLocation());
 		
 		try {
 			distance = roadbook.getTotalDistance();
 		} catch (IndexOutOfBoundsException e) {
 			distance = 0.0f;
 		}
 	    distanceTextView.setText(String.format("%.2f", distance));
 	}
 	
 // Toast
    
  private void toastMessage(String message) {
	    Toast toast = Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT);
	    if (toast != null) {
	    	toast.show();		
	    }
  }
}
