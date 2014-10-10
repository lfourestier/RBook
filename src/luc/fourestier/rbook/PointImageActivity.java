package luc.fourestier.rbook;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.v4.app.NavUtils;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;

public class PointImageActivity extends Activity {
	private ImageView pointImageView;

	private RoadBook currentRoadBook = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_point_image);
		setupActionBar();
		
		currentRoadBook = MainActivity.currentRoadBook;
		
		pointImageView = (ImageView) findViewById(R.id.point_point_image);
		RoadPoint roadpoint = currentRoadBook.getCurrentPoint();
 	    String path = roadpoint.getImage();
 	    if (!path.isEmpty()) {
 	        Bitmap bitmap = BitmapFactory.decodeFile(path);
 	       pointImageView.setImageBitmap(bitmap);
 	    }
 	    
		setTitle(currentRoadBook.getBookName() + " - #" + roadpoint.getNumber());
	}

	private void setupActionBar() {
		getActionBar().setDisplayHomeAsUpEnabled(true);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.point_image, menu);
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
	
	public void onClick(View v) {
		//NavUtils.navigateUpFromSameTask(this);
    }
	
}
