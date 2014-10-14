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
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class RoadPointActivity extends Activity {
	private TextView directionTextView; // TODO Use "icon_line" relative layout
										// instead of fixed layout.
	private TextView distancetonextTextView;
	private ImageView pointImageView;
	private TextView descriptionTextView;
	private TextView kilometerTextView;
	private TextView partialTextView;
	private TextView numberTextView;
	private Button previousButton;
	private Button nextButton;
	private Button mapButton;
	private Button albumButton;

	private Speech theSpeechEngine = null;
	private RoadBook currentRoadBook = null;
	private PictManager thePictManager = null;

	private String pointImageType;

// Activity	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_road_point);
		// Show the Up button in the action bar.
		setupActionBar();

		try {
			theSpeechEngine = MainActivity.theSpeechEngine;
			currentRoadBook = MainActivity.currentRoadBook;
			thePictManager = MainActivity.thePictManager;
	
			directionTextView = (TextView) findViewById(R.id.point_direction_text);
			distancetonextTextView = (TextView) findViewById(R.id.point_distancetonext_text);
			pointImageView = (ImageView) findViewById(R.id.point_road_image);
			pointImageView.setOnClickListener(mPictListener);
			descriptionTextView = (TextView) findViewById(R.id.point_description_text);
			kilometerTextView = (TextView) findViewById(R.id.point_kilometer_text);
			partialTextView = (TextView) findViewById(R.id.point_partialkilometer_text);
			numberTextView = (TextView) findViewById(R.id.point_numbers_text);
			previousButton = (Button) findViewById(R.id.point_previous_button);
			previousButton.setOnClickListener(mPrevListener);
			previousButton.setEnabled(true);
			nextButton = (Button) findViewById(R.id.point_next_button);
			nextButton.setOnClickListener(mNextListener);
			nextButton.setEnabled(true);
			albumButton = (Button) findViewById(R.id.point_album_button);
			albumButton.setOnClickListener(malbumListener);
			mapButton = (Button) findViewById(R.id.point_map_button);
			mapButton.setOnClickListener(mmapListener);
	
			setTitle(currentRoadBook.getBookName());

			refreshRoadPoint(currentRoadBook);
		} catch (Exception e) {
			String message = "Error while loading! ";
			Log.d(TAG, message + e.getMessage() + " " + e.toString());
			toastMessage(message);
		}
	}
	
// Action bar
	
	private void setupActionBar() {

		getActionBar().setDisplayHomeAsUpEnabled(true);

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.road_point, menu);
	    MenuItem item = menu.findItem(R.id.action_point_speech);
	    if (!theSpeechEngine.speechOn) {
		    item.setIcon(R.drawable.ic_action_volume_muted);
		}
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case android.R.id.home:
			NavUtils.navigateUpFromSameTask(this);
			return true;
		case R.id.action_point_speech:
		    if (theSpeechEngine.speechOn) {
		    	theSpeechEngine.mute();
			    item.setIcon(R.drawable.ic_action_volume_muted);
			}
		    else {
		    	theSpeechEngine.unMute();
	        	item.setIcon(R.drawable.ic_action_volume_on);
			}
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

// Buttons	
	
	private OnClickListener mPictListener = new OnClickListener() {
		public void onClick(View v) {
			try {
				String speech = currentRoadBook.getCurrentPointSpeech(thePictManager);
				theSpeechEngine.speak(speech);
			} catch (Exception e) {
				Log.d(TAG, "Error while speech! " + e.getMessage());
			}
		}
	};

	private OnClickListener mPrevListener = new OnClickListener() {
		public void onClick(View v) {
			try {
				currentRoadBook.previous();
				refreshRoadPoint(currentRoadBook);
				nextButton.setEnabled(true);
			} catch (IndexOutOfBoundsException e) {
				previousButton.setEnabled(false);
			} catch (Exception e) {
				Log.d(TAG,
						"Error while refreshing point! " + e.getMessage());
				// TODO: handle exception
			}
		}
	};

	private OnClickListener mNextListener = new OnClickListener() {
		public void onClick(View v) {
			try {
				String speech = currentRoadBook.getNextPointSpeech(thePictManager);
				currentRoadBook.next();
				refreshRoadPoint(currentRoadBook);
				theSpeechEngine.speak(speech);
				previousButton.setEnabled(true);
			} catch (IndexOutOfBoundsException e) {
				nextButton.setEnabled(false);
			} catch (Exception e) {
				Log.d(TAG,
						"Error while refreshing point! " + e.getMessage());
				// TODO: handle exception
			}
		}
	};

	private OnClickListener mmapListener = new OnClickListener() {
		public void onClick(View v) {
			toastMessage("Maps are coming soon!");
	    }
	};

	private OnClickListener malbumListener = new OnClickListener() {
		public void onClick(View v) {
        	try {
        		if (!currentRoadBook.getCurrentPoint().getImage().isEmpty()) {
	        	    Intent intent = new Intent(RoadPointActivity.this, PointImageActivity.class);
	        	    startActivity(intent);
        		}
        		else {
        			toastMessage("No Photo for that point!"); // TODO Change to @string for language compatibility
        		}
			} 
        	catch (Exception e) {
				Log.e(TAG, "Cannot load point image: " + e.getMessage());
				toastMessage("Cannot load point image!");
			}
	    }
	};
	
// Road point	
	
	private void refreshRoadPoint(RoadBook roadbook) {
		Float distance = 0.0f;
		RoadPoint roadpoint = roadbook.getCurrentPoint();
		
		directionTextView.setText(roadpoint.getDirection());
		
		try {
			distance = roadbook.getDistanceToNext();
		} catch (IndexOutOfBoundsException e) {
			distance = 0.0f;
		}
		distancetonextTextView.setText(String.format("%.2f", distance));
		
		pointImageType = roadpoint.getType();
		try {
			String path = thePictManager.getPict(pointImageType, thePictManager.RESOLUTION_FULL);
            Bitmap bitmap = BitmapFactory.decodeFile(path);
            pointImageView.setImageBitmap(bitmap);
		}
		catch (IndexOutOfBoundsException e) {
			pointImageView.setImageResource(R.drawable.question_mark_full);
		}
		
		descriptionTextView.setText(roadpoint.getDescription());
		kilometerTextView.setText(String.format("%.2f", roadpoint.getKilometer()));

		try {
			distance = roadbook.getDistanceFromPrevious();
		} catch (IndexOutOfBoundsException e) {
			distance = 0.0f;
		}
		partialTextView.setText(String.format("%.2f", distance));
		numberTextView.setText(Integer.toString(roadpoint.getNumber()) + "/" + Integer.toString(roadbook.getPointCount()));
	}
	
// Toast
    private static final String TAG = RoadPointActivity.class.getSimpleName();
    
    private void toastMessage(String message) {
	    Toast toast = Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT);
	    if (toast != null) {
	    	toast.show();		
	    }
    }
}
