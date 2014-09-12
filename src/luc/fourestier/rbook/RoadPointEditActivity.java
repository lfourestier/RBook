package luc.fourestier.rbook;

import android.app.Activity;
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

public class RoadPointEditActivity extends Activity {
	private EditText directionEditText; 
	private TextView distancetonextTextView;
	private ImageView pointImageView;
	private EditText descriptionEditText;
	private EditText kilometerEditText;
	private TextView partialTextView;
	private Button previousButton;
	private Button nextButton;
	private Button mapButton;
	private Button albumButton;
	
	private BookManager theBookManager = null;
	private RoadBook currentRoadBook;
	private String currentBookName;

	private String pointImageType;
	private final String drawablePrefix = "drawable";

// Activity
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_road_point_edit);
		setupActionBar();

		theBookManager = MainActivity.theBookManager;
		currentRoadBook = MainActivity.currentRoadBook;
		currentBookName = MainActivity.currentBookName;

		directionEditText = (EditText) findViewById(R.id.point_edit_direction_text);
		distancetonextTextView = (TextView) findViewById(R.id.point_edit_distancetonext_text);
		pointImageView = (ImageView) findViewById(R.id.point_edit_road_image);
		descriptionEditText = (EditText) findViewById(R.id.point_edit_description_text);
		kilometerEditText = (EditText) findViewById(R.id.point_edit_kilometer_text);
		partialTextView = (TextView) findViewById(R.id.point_edit_partialkilometer_text);
		previousButton = (Button) findViewById(R.id.point_edit_previous_button);
		previousButton.setOnClickListener(mPrevListener);
		previousButton.setEnabled(false);
		nextButton = (Button) findViewById(R.id.point_edit_next_button);
		nextButton.setOnClickListener(mNextListener);
		nextButton.setEnabled(true);
		albumButton = (Button) findViewById(R.id.point_edit_album_button);
		albumButton.setOnClickListener(malbumListener);
		mapButton = (Button) findViewById(R.id.point_edit_map_button);
		mapButton.setOnClickListener(mmapListener);

		setTitle(currentBookName);

		currentRoadBook.addNewPointBefore();
	}

// Action bar
	
	private void setupActionBar() {

		getActionBar().setDisplayHomeAsUpEnabled(true);

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.road_point_edit, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case android.R.id.home:
			NavUtils.navigateUpFromSameTask(this);
			return true;
        case R.id.action_save:
        	setRoadPoint();
		    theBookManager.saveRoadBook(currentRoadBook);
			NavUtils.navigateUpFromSameTask(this);
            return true;
		}
		return super.onOptionsItemSelected(item);
	}

// Buttons
	
	private OnClickListener mPrevListener = new OnClickListener() {
		public void onClick(View v) {
			setRoadPoint();
			try {
				currentRoadBook.previous();
				RefreshRoadPoint();
				nextButton.setEnabled(true);
			} catch (IndexOutOfBoundsException e) {
				previousButton.setEnabled(false);
			} catch (Exception e) {
				Log.d("RoadPointActivity", "Error while refreshing point! " + e.getMessage());
				// TODO: handle exception
			}
		}
	};

	private OnClickListener mNextListener = new OnClickListener() {
		public void onClick(View v) {
			setRoadPoint();
			try {
				currentRoadBook.next();
				RefreshRoadPoint();
			} catch (IndexOutOfBoundsException e) {
				currentRoadBook.addNewPointAfter();
				currentRoadBook.next();
				RefreshRoadPoint();
			} catch (Exception e) {
				Log.d("RoadPointActivity", "Error while refreshing point! " + e.getMessage());
				// TODO: handle exception
			}
		}
	};

	private OnClickListener mmapListener = new OnClickListener() {
		public void onClick(View v) {
//			LayoutInflater inflater = (LayoutInflater) v.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
//		    View view = inflater.inflate(R.layout.,
//		                                   (ViewGroup) findViewById(R.id.image_toast));

			toastMessage("Maps are coming soon!");
	    }
	};

	private OnClickListener malbumListener = new OnClickListener() {
		public void onClick(View v) {
//			LayoutInflater inflater = (LayoutInflater) v.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
//		    View view = inflater.inflate(R.layout.,
//		                                   (ViewGroup) findViewById(R.id.image_toast));

			toastMessage("Photos are coming soon!");
	    }
	};
	
// Road point	

	private void RefreshRoadPoint() {
		Float distance = 0.0f;
		directionEditText.setText(currentRoadBook.getCurrentPoint().getDirection());
		
		try {
			distance = currentRoadBook.getDistanceToNext();
		} catch (IndexOutOfBoundsException e) {
			distance = 0.0f;
		}
		distancetonextTextView.setText(String.format("%.2f", distance));
		
		pointImageType = currentRoadBook.getCurrentPoint().getType();
		int id = getResources().getIdentifier(pointImageType, drawablePrefix,
				getPackageName());
		if (id != 0) {
			pointImageView.setImageResource(id);
		} else {
			pointImageView.setImageResource(R.drawable.question_mark);
		}
		descriptionEditText.setText(currentRoadBook.getCurrentPoint().getDescription());
		kilometerEditText.setText(String.format("%.2f", currentRoadBook.getCurrentPoint().getKilometer()));

		try {
			distance = currentRoadBook.getDistanceFromPrevious();
		} catch (IndexOutOfBoundsException e) {
			distance = 0.0f;
		}
		partialTextView.setText(String.format("%.2f", distance));
	}
		
	private void setRoadPoint() {
		RoadPoint currentRoadPoint = currentRoadBook.getCurrentPoint();
		
		currentRoadPoint.setDirection(directionEditText.getText().toString());
		currentRoadPoint.setType("ra4_exit2"); // TODO Select images here!
		currentRoadPoint.setDescription(descriptionEditText.getText().toString());
		currentRoadPoint.setKilometer(Float.parseFloat(kilometerEditText.getText().toString()));
	}
		
// Toast
    
    private void toastMessage(String message) {
	    Toast toast = Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT);
	    if (toast != null) {
	    	toast.show();		
	    }
    }

}
