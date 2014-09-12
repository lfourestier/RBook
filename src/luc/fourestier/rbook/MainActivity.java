package luc.fourestier.rbook;

import java.util.ArrayList;

import luc.fourestier.rbook.AddDialogFragment.AddDialogListener;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.DialogFragment;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.Toast;

public class MainActivity extends FragmentActivity implements AddDialogListener {
	private ListView bookListView;

	private ArrayList<String> bookListArray;
	private BookListAdapter listAdapter;
	
	public static BookManager theBookManager = null;
	public static RoadBook currentRoadBook = null; // TODO Should not be static!
	public static String currentBookName = null;

// Native library
	
	static {
		System.loadLibrary("RBook");
	}
	
// Activity
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		bookListView = (ListView) findViewById(R.id.road_book_list);

		try {
			if (theBookManager == null) {
				String sdcard = Environment.getExternalStorageDirectory()
						.getPath();
				Log.v("MAIN", "External storage dir: " + sdcard);
				theBookManager = BookManager.Create(sdcard);
			}

			bookListArray = theBookManager.getRoadBookList();
			if (currentBookName == null) {
				currentBookName = bookListArray.get(0);
			}


			listAdapter = new BookListAdapter(this, bookListArray);
			bookListView.setAdapter(listAdapter);
			bookListView.setOnItemClickListener(mListViewListener);
		} catch (Exception e) {
			Log.e("MAIN", "BookManager creation failed");
		}
	}

// Action bar
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
	    // Handle presses on the action bar items
	    switch (item.getItemId()) {
	        case R.id.action_settings:
	            //openSearch();
	            return true;
	        case R.id.action_add:
			    showAddDialog();
	            return true;
	        default:
	            return super.onOptionsItemSelected(item);
	    }
	}

// Roadbook ListView
	
	private AdapterView.OnItemClickListener mListViewListener = new AdapterView.OnItemClickListener() {

		@Override
		public void onItemClick(AdapterView<?> parent, final View view,
				int position, long id) {
			try {
				// Save the last file name
				currentBookName = (String) parent.getItemAtPosition(position);

				// Unload the old one
				if (currentRoadBook != null) {
					theBookManager.releaseRoadBook(currentRoadBook);
					currentRoadBook = null;
				}

				// Load the new road book
				currentRoadBook = theBookManager.getRoadBook(currentBookName);

				Intent intent = new Intent(MainActivity.this, RoadBookActivity.class);
				startActivity(intent);

			} catch (Exception e) {
				// TODO: handle exception
			}
		}
	};

// New roadbook dialog
	
   public void showAddDialog() {
        DialogFragment dialog = new AddDialogFragment();
        dialog.show(getSupportFragmentManager(), "AddDialogFragment");
    }

    @Override
    public void onDialogPositiveClick(DialogFragment dialog) {
		currentBookName = ((AddDialogFragment) dialog).roadBookName;

		if (!currentBookName.isEmpty()) {
			if (currentRoadBook != null) {
				theBookManager.releaseRoadBook(currentRoadBook);
				currentRoadBook = null;
			}
			currentRoadBook = theBookManager.createRoadBook(currentBookName);
			
//	    	bookListArray.add(currentBookName);
//	    	listAdapter.notifyDataSetChanged();
	    	
			Intent intent = new Intent(MainActivity.this, RoadBookEditActivity.class);
			startActivity(intent);
		}
		else {
			toastMessage(getString(R.string.warning_name_empty));
		}
    }
   
// Toast
    
    private void toastMessage(String message) {
	    Toast toast = Toast.makeText(getApplicationContext(), message, Toast.LENGTH_LONG);
	    if (toast != null) {
	    	toast.show();		
	    }
    }
}

