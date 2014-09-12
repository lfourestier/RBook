package luc.fourestier.rbook;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.NavUtils;
import android.text.InputFilter.LengthFilter;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
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
	private Button previousButton;
	private Button nextButton;
	private Button mapButton;
	private Button albumButton;

	private RoadBook currentRoadBook;
	private String currentBookName;

	private String pointImageType;
	private final String drawablePrefix = "drawable";

// Activity	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_road_point);
		// Show the Up button in the action bar.
		setupActionBar();

		currentRoadBook = MainActivity.currentRoadBook;
		currentBookName = MainActivity.currentBookName;

		directionTextView = (TextView) findViewById(R.id.point_direction_text);
		distancetonextTextView = (TextView) findViewById(R.id.point_distancetonext_text);
		pointImageView = (ImageView) findViewById(R.id.point_road_image);
		descriptionTextView = (TextView) findViewById(R.id.point_description_text);
		kilometerTextView = (TextView) findViewById(R.id.point_kilometer_text);
		partialTextView = (TextView) findViewById(R.id.point_partialkilometer_text);
		previousButton = (Button) findViewById(R.id.point_previous_button);
		previousButton.setOnClickListener(mPrevListener);
		previousButton.setEnabled(false);
		nextButton = (Button) findViewById(R.id.point_next_button);
		nextButton.setOnClickListener(mNextListener);
		nextButton.setEnabled(true);
		albumButton = (Button) findViewById(R.id.point_album_button);
		albumButton.setOnClickListener(malbumListener);
		mapButton = (Button) findViewById(R.id.point_map_button);
		mapButton.setOnClickListener(mmapListener);

		setTitle(currentBookName);

		try {
			RefreshRoadPoint();
		} catch (Exception e) {
			Log.d("RoadPointActivity",
					"Error while refreshing point! " + e.getMessage());
			// TODO: handle exception
		}
	}

// Action bar
	
	private void setupActionBar() {

		getActionBar().setDisplayHomeAsUpEnabled(true);

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.road_point, menu);
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

// Buttons	
	
	private OnClickListener mPrevListener = new OnClickListener() {
		public void onClick(View v) {
			try {
				currentRoadBook.previous();
				RefreshRoadPoint();
				nextButton.setEnabled(true);
			} catch (IndexOutOfBoundsException e) {
				previousButton.setEnabled(false);
			} catch (Exception e) {
				Log.d("RoadPointActivity",
						"Error while refreshing point! " + e.getMessage());
				// TODO: handle exception
			}
		}
	};

	private OnClickListener mNextListener = new OnClickListener() {
		public void onClick(View v) {
			try {
				currentRoadBook.next();
				RefreshRoadPoint();
				previousButton.setEnabled(true);
			} catch (IndexOutOfBoundsException e) {
				nextButton.setEnabled(false);
			} catch (Exception e) {
				Log.d("RoadPointActivity",
						"Error while refreshing point! " + e.getMessage());
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
		directionTextView.setText(currentRoadBook.getCurrentPoint().getDirection());
		
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
		descriptionTextView.setText(currentRoadBook.getCurrentPoint().getDescription());
		kilometerTextView.setText(String.format("%.2f", currentRoadBook.getCurrentPoint().getKilometer()));

		try {
			distance = currentRoadBook.getDistanceFromPrevious();
		} catch (IndexOutOfBoundsException e) {
			distance = 0.0f;
		}
		partialTextView.setText(String.format("%.2f", distance));
	}
	
// Toast
    
    private void toastMessage(String message) {
	    Toast toast = Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT);
	    if (toast != null) {
	    	toast.show();		
	    }
    }
}
