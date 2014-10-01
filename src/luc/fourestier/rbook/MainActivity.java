package luc.fourestier.rbook;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Locale;

import luc.fourestier.rbook.AddDialogFragment.AddDialogListener;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.speech.tts.TextToSpeech;
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
	
	public static Speech theSpeechEngine = null;
	public static BookManager theBookManager = null;
	public static PictManager thePictManager = null;
	public static RoadBook currentRoadBook = null;

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
			AssetManager assetManager = getAssets();
			
			// Initialize the book manager
			String sdcard = Environment.getExternalStorageDirectory().getPath();
			if (theBookManager == null) {
				Log.v(TAG, "External storage dir: " + sdcard);
				theBookManager = BookManager.Create(sdcard);
			}

			// Initialize the picts
			if (thePictManager == null) {
				InputStream input = assetManager.open("pict.zip");
				String tempfile = sdcard + "/RBook/Temp/pict.zip";
				InputStreamToFile(input, tempfile);
				thePictManager = PictManager.create(tempfile, sdcard + "/RBook/"); // TODO No hardcoded path here!!
			}

			try {
				// Import file if activity was triggered by an external intent
				Intent intent = getIntent();
				if (intent != null) {
					String action = intent.getAction();
					
					if ((action != null) && (action.compareTo(Intent.ACTION_VIEW) == 0)) {
						Log.v(TAG, "ACTION_VIEW intent detected: " + intent.getDataString() + " : " + intent.getType());
						
						String scheme = intent.getScheme();
						Uri uri = intent.getData();
						ContentResolver resolver = getContentResolver();
						
						// We will temporarily copy the file here below before definitive import.
						String tempfile = sdcard + "/RBook/Temp/"; // TODO No hardcoded path here!!!
						String filename = null;
						
						if (scheme.compareTo(ContentResolver.SCHEME_CONTENT) == 0) {
							filename = getContentName(resolver, uri);
							
							InputStream input = resolver.openInputStream(uri);
							tempfile = tempfile + filename; 
							InputStreamToFile(input, tempfile);
						}
						else if (scheme.compareTo(ContentResolver.SCHEME_FILE) == 0) {
							filename = uri.getLastPathSegment();
							
							InputStream input = resolver.openInputStream(uri);
							tempfile = tempfile + filename; 
							InputStreamToFile(input, tempfile);
						}
						else if (scheme.compareTo("http") == 0) {
							// TODO Import from website!
						}
						
						// Import the file definitively then
						if (filename != null) {
							try {
								theBookManager.importRoadBook(tempfile, filename.split("\\.")[0], false);
							} catch (java.lang.IllegalArgumentException e) {
								toastMessage("Roadbook already exist in the list!");
								// TODO Add dialog to change name or overwrite.
							}
						}
					}
				}
			}
			catch (Exception e) {
				Log.e(TAG, "Error while importing: " + e.getMessage());
				toastMessage("Import failed!");
			}
			
			// Initialize the view elements
			bookListView = (ListView) findViewById(R.id.road_book_list);

			bookListArray = theBookManager.getRoadBookList();

			listAdapter = new BookListAdapter(this, bookListArray);
			bookListView.setAdapter(listAdapter);
			bookListView.setOnItemClickListener(mListViewListener);
			
			if (currentRoadBook != null) {
				theBookManager.releaseRoadBook(currentRoadBook);
				currentRoadBook = null;
			}

			// Initializes text to speech
			if (theSpeechEngine == null) {
				theSpeechEngine = new Speech(); 
				theSpeechEngine.initialize(getApplicationContext()); // TODO Should be configurable in the settings
			}
			
		} catch (Exception e) {
			Log.e(TAG, "Error while starting: " + e.getMessage());
			toastMessage("Error while starting!");
			theBookManager = null;
			thePictManager = null;
		}
	}
	
	@Override
	public void onConfigurationChanged (Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		if (newConfig != null) {
			Log.v(TAG, "Orientation: " + Integer.toString(newConfig.orientation));
		}
	}
	
	@Override
	public void onDestroy() {
		theSpeechEngine.destroy();
		super.onDestroy();
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
				Log.e(TAG, "Error while loading: " + e.getMessage());
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
    
    private static final String TAG = MainActivity.class.getSimpleName();
    
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
			Log.e(TAG, "InputStreamToFile exception: " + e.getMessage());
		}
    }
}

