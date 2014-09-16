package luc.fourestier.rbook;

import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.NavUtils;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.GridView;
import android.widget.Toast;

public class RoadPointSelectActivity extends Activity {
	private GridView gridview;
	private String[] pointSelectionArray = {
        "question_mark", "ra4_exit2",
        "start", "t_left",
        "t_right", "x4_left",
        "x4_right", "x4_straight",
        "y_left", "y_right",
        "finish"
	};
	
	private RoadBook currentRoadBook = null;
	
// Activity
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_road_point_select);
		setupActionBar();
		
		currentRoadBook = MainActivity.currentRoadBook;
		
		gridview = (GridView) findViewById(R.id.point_select_gridview);
		RoadPointImageAdapter adapter = new RoadPointImageAdapter(this);
		adapter.mPointNames = pointSelectionArray;
		gridview.setAdapter(adapter);
		gridview.setOnItemClickListener(mGridItemListener);
	}

// Action bar
	
	private void setupActionBar() {
		getActionBar().setDisplayHomeAsUpEnabled(true);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.road_point_select, menu);
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

	private OnItemClickListener mGridItemListener = new OnItemClickListener() {
		public void onItemClick(AdapterView<?> parent, View v, int position, long id) {
			currentRoadBook.getCurrentPoint().setType(pointSelectionArray[position]);
			NavUtils.navigateUpFromSameTask(RoadPointSelectActivity.this);
		}
	};
	
// Toast
    
    private void toastMessage(String message) {
	    Toast toast = Toast.makeText(getApplicationContext(), message, Toast.LENGTH_SHORT);
	    if (toast != null) {
	    	toast.show();		
	    }
    }

}
