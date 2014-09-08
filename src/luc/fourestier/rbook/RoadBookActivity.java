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
import android.widget.TextView;

public class RoadBookActivity extends Activity {
    private TextView titleTextView;
    private TextView descriptionTextView;
    private TextView locationTextView;
    private TextView distanceTextView;
    private Button startButton;

    private RoadBook currentRoadBook = null;
    private String currentBookName;
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_road_book);
		setupActionBar();
		
		currentRoadBook = MainActivity.currentRoadBook;
		currentBookName = MainActivity.currentBookName;
		
    	titleTextView = (TextView) findViewById(R.id.book_name_text);
    	descriptionTextView = (TextView) findViewById(R.id.book_description_text);
    	locationTextView = (TextView) findViewById(R.id.book_location_text);
    	distanceTextView = (TextView) findViewById(R.id.book_distance_text);
    	startButton = (Button) findViewById(R.id.book_start_button);
    	startButton.setOnClickListener(mStartListener);
    	
    	setTitle(currentBookName);
    	
	    titleTextView.setText(currentRoadBook.getBookTitle());
	    descriptionTextView.setText(currentRoadBook.getBookDescription());
	    locationTextView.setText(currentRoadBook.getBookLocation());
	    distanceTextView.setText(String.format("%.1f", currentRoadBook.getBookTotalDistance()));
	}

	/**
	 * Set up the {@link android.app.ActionBar}.
	 */
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
			// This ID represents the Home or Up button. In the case of this
			// activity, the Up button is shown. Use NavUtils to allow users
			// to navigate up one level in the application structure. For
			// more details, see the Navigation pattern on Android Design:
			//
			// http://developer.android.com/design/patterns/navigation.html#up-vs-back
			//
			NavUtils.navigateUpFromSameTask(this);
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

    private OnClickListener mStartListener = new OnClickListener() {
        public void onClick(View v) {
        	try {
        	    Intent intent = new Intent(RoadBookActivity.this, RoadPointActivity.class);
        	    //intent.putExtra(EXTRA_MESSAGE, message);
        	    startActivity(intent);
			} 
        	catch (Exception e) {
				// TODO: handle exception
			}
        }
    };
}
