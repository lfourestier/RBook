package luc.fourestier.rbook;

import java.io.File;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.v4.app.NavUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
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
    private Button photoButton;
	
	private BookManager theBookManager = null;
	private RoadBook currentRoadBook = null;
	private PictManager thePictManager = null;

	private String pointImageType;
	private final String drawablePrefix = "drawable";

	private static final int CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE = 100;

// Activity
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_road_point_edit);
		setupActionBar();

		try {
			theBookManager = MainActivity.theBookManager;
			currentRoadBook = MainActivity.currentRoadBook;
			thePictManager = MainActivity.thePictManager;
	
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
	    	photoButton = (Button) findViewById(R.id.point_edit_photo_button);
	    	photoButton.setOnClickListener(mPhotoListener);
	
			setTitle(currentRoadBook.getBookName());
	
			try {
				refreshRoadPointView(currentRoadBook);
			} catch (IndexOutOfBoundsException e) {
				currentRoadBook.addNewPointAfter();
				refreshRoadPointView(currentRoadBook);
			}
		
		} catch (Exception e) {
			Log.e(TAG, "Error while loading!" + e.getMessage());
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
				Log.e(TAG, "Error while goign back!" + e.getMessage());
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
				Log.e(TAG, "Error while saving!" + e.getMessage());
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
				Log.e(TAG, "Error while going next!" + e.getMessage());
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
				Log.e(TAG, "Error while going previous!" + e.getMessage());
				toastMessage("Oups! Cannot load!");
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
	        	    Intent intent = new Intent(RoadPointEditActivity.this, PointEditImageActivity.class);
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
	
 // Photo button
	
 	private OnClickListener mPhotoListener = new OnClickListener() {
         public void onClick(View v) {
         	try {
         		setValueFromRoadPointView(currentRoadBook.getCurrentPoint());
         		
         	    // create Intent to take a picture and return control to the calling application
         	    Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);

         	    Uri fileUri = Uri.fromFile(new File(currentRoadBook.getCurrentPoint().createImagePath()));
         	    intent.putExtra(MediaStore.EXTRA_OUTPUT, fileUri); 

         	    // start the image capture Intent
         	    startActivityForResult(intent, CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE);
 			} 
         	catch (Exception e) {
 				Log.e(TAG, "Cannot take road point picture: " + e.getMessage());
 				toastMessage("Oups! Cannot take picture!");
 			}
         }
     };
     
     @Override
     protected void onActivityResult(int requestCode, int resultCode, Intent data) {
         if (requestCode == CAPTURE_IMAGE_ACTIVITY_REQUEST_CODE) {
             if (resultCode == RESULT_OK) {
            	 if (data != null) {
            		 toastMessage("Image saved to: " + data.getData());
                 }
                 currentRoadBook.getCurrentPoint().setImage(currentRoadBook.getCurrentPoint().createImagePath());
                 refreshRoadPointView(currentRoadBook);
             } 
         }
     }
     
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
		try {
			String path = thePictManager.getPict(pointImageType, thePictManager.RESOLUTION_FULL);
            Bitmap bitmap = BitmapFactory.decodeFile(path);
            pointImageButton.setImageBitmap(bitmap);
		}
		catch (IndexOutOfBoundsException e) {
			pointImageButton.setImageResource(R.drawable.question_mark_full);
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
    private static final String TAG = RoadPointEditActivity.class.getSimpleName();
    
    private void toastMessage(String message) {
	    Toast toast = Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT);
	    if (toast != null) {
	    	toast.show();		
	    }
    }

}
