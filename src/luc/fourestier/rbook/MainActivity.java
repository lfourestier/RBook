package luc.fourestier.rbook;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

import luc.fourestier.rbook.AddDialogFragment.AddDialogListener;
import android.content.ContentResolver;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
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

// Native library
	
	static {
		System.loadLibrary("RBook");
	}
	
// Activity
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		try {
			String sdcard = Environment.getExternalStorageDirectory().getPath();
			if (theBookManager == null) {
				Log.v("MAIN", "External storage dir: " + sdcard);
				theBookManager = BookManager.Create(sdcard);
			}

			Intent intent = getIntent();
			String action = intent.getAction();
			
			if (action.compareTo(Intent.ACTION_VIEW) == 0) {
				String scheme = intent.getScheme();
				ContentResolver resolver = getContentResolver();
				
				if (scheme.compareTo(ContentResolver.SCHEME_CONTENT) == 0) {
					Uri uri = intent.getData();
					String name = getContentName(resolver, uri);
					
					Log.v("LFOR" , "Content intent detected: " + action + " : " + intent.getDataString() + " : " + intent.getType() + " : " + name);
					InputStream input = resolver.openInputStream(uri);
					String importfilepath = sdcard + "/RBook/Temp/" + name; // TODO No hardcoded path here!!!
					InputStreamToFile(input, importfilepath);
					try {
						theBookManager.importRoadBook(importfilepath, name.split(".")[0], false); // TODO name.split might fail => check Error!!
						Log.v("LFOR" , "after importRoadBook");
					} catch (java.lang.IllegalArgumentException e) {
						toastMessage("Roadbook already exist in the list!");
					}
				}
				else if (scheme.compareTo(ContentResolver.SCHEME_FILE) == 0) {
					Uri uri = intent.getData();
					String name = uri.getLastPathSegment();
					
					Log.v("LFOR" , "File intent detected: " + action + " : " + intent.getDataString() + " : " + intent.getType() + " : " + name);
					InputStream input = resolver.openInputStream(uri);
					String importfilepath = sdcard + "/RBook/Temp/" + name; // TODO No hardcoded path here!!!
					InputStreamToFile(input, importfilepath);
					try {
						theBookManager.importRoadBook(importfilepath, name.split(".")[0], false); // TODO name.split might fail => check Error!!
					} catch (java.lang.IllegalArgumentException e) {
						toastMessage("Roadbook already exist in the list!");
					}
				}
				else if (scheme.compareTo("http") == 0) {
					// TODO Import from website!
				}
			}
			
			bookListView = (ListView) findViewById(R.id.road_book_list);

			bookListArray = theBookManager.getRoadBookList();

			listAdapter = new BookListAdapter(this, bookListArray);
			bookListView.setAdapter(listAdapter);
			bookListView.setOnItemClickListener(mListViewListener);
			
			if (currentRoadBook != null) {
				theBookManager.releaseRoadBook(currentRoadBook);
				currentRoadBook = null;
			}

		} catch (Exception e) {
			Log.e("MainActivity", "Error while starting: " + e.getMessage());
			toastMessage("Error while starting!");
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
	        case R.id.action_help:
				Intent intent = new Intent(MainActivity.this, HelpActivity.class);
				startActivity(intent);
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
				String bookName = (String) parent.getItemAtPosition(position);

				// Unload the old one
				if (currentRoadBook != null) {
					theBookManager.releaseRoadBook(currentRoadBook);
					currentRoadBook = null;
				}

				// Load the new road book
				currentRoadBook = theBookManager.getRoadBook(bookName);

				Intent intent = new Intent(MainActivity.this, RoadBookActivity.class);
				startActivity(intent);

			} catch (Exception e) {
				Log.e("MainActivity", "Error while loading: " + e.getMessage());
				toastMessage("Oups! Cannot load!");
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
		String bookName = ((AddDialogFragment) dialog).roadBookName;

		if (!bookName.isEmpty()) {
			if (currentRoadBook != null) {
				theBookManager.releaseRoadBook(currentRoadBook);
				currentRoadBook = null;
			}
			currentRoadBook = theBookManager.createRoadBook(bookName);
			
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
    
// Utils
    
    private String getContentName(ContentResolver resolver, Uri uri){
        Cursor cursor = resolver.query(uri, null, null, null, null);
        cursor.moveToFirst();
        int nameIndex = cursor.getColumnIndex(MediaStore.MediaColumns.DISPLAY_NAME);
        if (nameIndex >= 0) {
            return cursor.getString(nameIndex);
        } else {
            return null;
        }
    }
    
    private void InputStreamToFile(InputStream in, String file) {
    	try {
	    	OutputStream out = new FileOutputStream(new File(file));
	
			int size = 0;
			byte[] buffer = new byte[1024];
		
			while ((size = in.read(buffer)) != -1) {
				out.write(buffer, 0, size);
			}
			
			out.close();
    	}
		catch (Exception e) {
			Log.e("MainActivity", "InputStreamToFile exception: " + e.getMessage());
		}
    }
}

