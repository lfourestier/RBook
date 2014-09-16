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
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class RoadPointEditActivity extends Activity {
	private EditText directionEditText; 
	private TextView distancetonextTextView;
	private ImageButton pointImageButton;
	private EditText descriptionEditText;
	private EditText kilometerEditText;
	private TextView partialTextView;
	private TextView numberTextView;
	private Button previousButton;
	private Button nextButton;
	private Button mapButton;
	private Button albumButton;
	
	private BookManager theBookManager = null;
	private RoadBook currentRoadBook = null;

	private String pointImageType;
	private final String drawablePrefix = "drawable";

// Activity
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_road_point_edit);
		setupActionBar();

		try {
			theBookManager = MainActivity.theBookManager;
			currentRoadBook = MainActivity.currentRoadBook;
	
			directionEditText = (EditText) findViewById(R.id.point_edit_direction_text);
			distancetonextTextView = (TextView) findViewById(R.id.point_edit_distancetonext_text);
			pointImageButton = (ImageButton) findViewById(R.id.point_edit_road_image);
			pointImageButton.setOnClickListener(mPointImageListener);
			descriptionEditText = (EditText) findViewById(R.id.point_edit_description_text);
			kilometerEditText = (EditText) findViewById(R.id.point_edit_kilometer_text);
			partialTextView = (TextView) findViewById(R.id.point_edit_partialkilometer_text);
			numberTextView = (TextView) findViewById(R.id.point_edit_numbers_text);
			previousButton = (Button) findViewById(R.id.point_edit_previous_button);
			previousButton.setOnClickListener(mPrevListener);
			previousButton.setEnabled(true);
			nextButton = (Button) findViewById(R.id.point_edit_next_button);
			nextButton.setOnClickListener(mNextListener);
			nextButton.setEnabled(true);
			albumButton = (Button) findViewById(R.id.point_edit_album_button);
			albumButton.setOnClickListener(malbumListener);
			mapButton = (Button) findViewById(R.id.point_edit_map_button);
			mapButton.setOnClickListener(mmapListener);
	
			setTitle(currentRoadBook.getBookName());
	
			try {
				refreshRoadPointView(currentRoadBook);
			} catch (IndexOutOfBoundsException e) {
				currentRoadBook.addNewPointAfter();
				refreshRoadPointView(currentRoadBook);
			}
		
		} catch (Exception e) {
			Log.e("RoadPointEditActivity", "Error while loading!" + e.getMessage());
			toastMessage("Oups! Cannot load!");
		}
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
        	try {
	        	setValueFromRoadPointView(currentRoadBook.getCurrentPoint());
				NavUtils.navigateUpFromSameTask(this);
			} catch (Exception e) {
				Log.e("RoadPointEditActivity", "Error while goign back!" + e.getMessage());
				toastMessage("Oups! Cannot load!");
			}
			return true;
        case R.id.action_point_save:
        	try {
	        	setValueFromRoadPointView(currentRoadBook.getCurrentPoint());
			    theBookManager.saveRoadBook(currentRoadBook);
	    	    Intent intent = new Intent(this, MainActivity.class);
				NavUtils.navigateUpTo(this, intent);
			} catch (Exception e) {
				Log.e("RoadPointEditActivity", "Error while saving!" + e.getMessage());
				toastMessage("Oups! Cannot save!");
			}
            return true;
        case R.id.action_point_add:
        	setValueFromRoadPointView(currentRoadBook.getCurrentPoint());
        	currentRoadBook.addNewPointBefore();
        	refreshRoadPointView(currentRoadBook);
            return true;
        case R.id.action_point_delete:
        	setValueFromRoadPointView(currentRoadBook.getCurrentPoint());
        	currentRoadBook.deleteCurrentPoint();
        	refreshRoadPointView(currentRoadBook);
            return true;
		}
		return super.onOptionsItemSelected(item);
	}

// Buttons
	
	private OnClickListener mPointImageListener = new OnClickListener() {
		public void onClick(View v) {
			setValueFromRoadPointView(currentRoadBook.getCurrentPoint());
    	    Intent intent = new Intent(RoadPointEditActivity.this, RoadPointSelectActivity.class);
    	    startActivity(intent);
	    }
	};

	private OnClickListener mPrevListener = new OnClickListener() {
		public void onClick(View v) {
			setValueFromRoadPointView(currentRoadBook.getCurrentPoint());
			try {
				currentRoadBook.previous();
				refreshRoadPointView(currentRoadBook);
				nextButton.setEnabled(true);
			} catch (IndexOutOfBoundsException e) {
				previousButton.setEnabled(false);
			} catch (Exception e) {
				Log.e("RoadPointEditActivity", "Error while going next!" + e.getMessage());
				toastMessage("Oups! Cannot load!");
			}
		}
	};

	private OnClickListener mNextListener = new OnClickListener() {
		public void onClick(View v) {
			setValueFromRoadPointView(currentRoadBook.getCurrentPoint());
			try {
				currentRoadBook.next();
				refreshRoadPointView(currentRoadBook);
				previousButton.setEnabled(true);
			} catch (IndexOutOfBoundsException e) {
				Float kilometer = currentRoadBook.getCurrentPoint().getKilometer();
				RoadPoint roadpoint = currentRoadBook.addNewPointAfter(); // Advance index!
				roadpoint.setKilometer(kilometer);
				refreshRoadPointView(currentRoadBook);
				previousButton.setEnabled(true);
			} catch (Exception e) {
				Log.e("RoadPointEditActivity", "Error while going previous!" + e.getMessage());
				toastMessage("Oups! Cannot load!");
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

	private void refreshRoadPointView(RoadBook roadbook) {
		Float distance = 0.0f;
		RoadPoint roadpoint = roadbook.getCurrentPoint();
		directionEditText.setText(roadpoint.getDirection());
		
		try {
			distance = roadbook.getDistanceToNext();
		} catch (IndexOutOfBoundsException e) {
			distance = 0.0f;
		}
		distancetonextTextView.setText(String.format("%.2f", distance));
		
		pointImageType = roadpoint.getType();
		int id = getResources().getIdentifier(pointImageType, drawablePrefix,
				getPackageName());
		if (id != 0) {
			pointImageButton.setImageResource(id);
		} else {
			pointImageButton.setImageResource(R.drawable.question_mark);
		}
		descriptionEditText.setText(roadpoint.getDescription());
		kilometerEditText.setText(String.format("%.2f", roadpoint.getKilometer()));

		try {
			distance = roadbook.getDistanceFromPrevious();
		} catch (IndexOutOfBoundsException e) {
			distance = 0.0f;
		}
		partialTextView.setText(String.format("%.2f", distance));
		numberTextView.setText(Integer.toString(roadpoint.getNumber()) + "/" + Integer.toString(roadbook.getPointCount()));
	}
		
	private void setValueFromRoadPointView(RoadPoint roadpoint) {
		roadpoint.setDirection(directionEditText.getText().toString());
		if (roadpoint.getType() == "") {
			roadpoint.setType("question_mark");
		}
		roadpoint.setDescription(descriptionEditText.getText().toString());
		String kilometerstring = kilometerEditText.getText().toString();
		kilometerstring = kilometerstring.replace(',', '.');
		roadpoint.setKilometer(Float.parseFloat(kilometerstring));
	}
		
// Toast
    
    private void toastMessage(String message) {
	    Toast toast = Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT);
	    if (toast != null) {
	    	toast.show();		
	    }
    }

}
