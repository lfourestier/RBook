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
import android.widget.ImageView;
import android.widget.TextView;

public class RoadPointActivity extends Activity {
	private TextView directionTextView; // TODO Use "icon_line" relative layout
										// instead of fixed layout.
	private TextView distancetonextTextView;
	private ImageView pointImageView;
	private TextView nameTextView;
	private TextView kilometerTextView;
	private TextView partialTextView;
	private Button previousButton;
	private Button nextButton;

	private RoadBook currentRoadBook;
	private String currentBookName;

	private String pointImageType;
	private final String drawablePrefix = "drawable";

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
		nameTextView = (TextView) findViewById(R.id.point_name_text);
		kilometerTextView = (TextView) findViewById(R.id.point_kilometer_text);
		partialTextView = (TextView) findViewById(R.id.point_partialkilometer_text);
		previousButton = (Button) findViewById(R.id.point_previous_button);
		previousButton.setOnClickListener(mPrevListener);
		previousButton.setEnabled(false);
		nextButton = (Button) findViewById(R.id.point_next_button);
		nextButton.setOnClickListener(mNextListener);
		nextButton.setEnabled(true);

		setTitle(currentBookName);

		try {
			RefreshRoadPoint();
		} catch (Exception e) {
			Log.d("RoadPointActivity",
					"Error while refreshing point! " + e.getMessage());
			// TODO: handle exception
		}
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
		getMenuInflater().inflate(R.menu.road_point, menu);
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

	private void RefreshRoadPoint() {
		Float distance = 0.0f;
		directionTextView.setText(currentRoadBook.getCurrentPointDirection());
		
		try {
			distance = currentRoadBook.getCurrentPointDistanceToNext();
		} catch (IndexOutOfBoundsException e) {
			distance = 0.0f;
		}
		distancetonextTextView.setText(String.format("%.2f", distance));
		
		pointImageType = currentRoadBook.getCurrentPointType();
		int id = getResources().getIdentifier(pointImageType, drawablePrefix,
				getPackageName());
		if (id != 0) {
			pointImageView.setImageResource(id);
		} else {
			pointImageView.setImageResource(R.drawable.question_mark);
		}
		nameTextView.setText(currentRoadBook.getCurrentPointName());
		kilometerTextView.setText(String.format("%.2f", currentRoadBook.getCurrentPointKilometer()));

		try {
			distance = currentRoadBook.getCurrentPointDistanceFromPrevious();
		} catch (IndexOutOfBoundsException e) {
			distance = 0.0f;
		}
		partialTextView.setText(String.format("%.2f", distance));
	}
}
