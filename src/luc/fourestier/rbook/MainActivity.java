package luc.fourestier.rbook;

import java.util.ArrayList;

import android.app.ActionBar;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;

public class MainActivity extends Activity {
	private ListView bookListView;

	private ArrayList<String> bookListArray;

	public static BookManager theBookManager = null;
	public static RoadBook currentRoadBook = null; // TODO Should not be static!
	public static String currentBookName = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		if (currentBookName == null) {
			currentBookName = getString(R.string.test_road_book);
		}

		bookListView = (ListView) findViewById(R.id.road_book_list);

		try {
			if (theBookManager == null) {
				String sdcard = Environment.getExternalStorageDirectory()
						.getPath();
				Log.v("MAIN", "External storage dir: " + sdcard);
				theBookManager = BookManager.Create(sdcard);
			}

			bookListArray = theBookManager.getRoadBookList();

			final BookListAdapter listadapter = new BookListAdapter(
					this, bookListArray);
			bookListView.setAdapter(listadapter);
			bookListView.setOnItemClickListener(mListViewListener);
		} catch (Exception e) {
			Log.e("MAIN", "BookManager creation failed");
		}
	}

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

				Intent intent = new Intent(MainActivity.this,
						RoadBookActivity.class);
				// intent.putExtra(EXTRA_MESSAGE, message);
				startActivity(intent);

			} catch (Exception e) {
				// TODO: handle exception
			}
		}
	};

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	static {
		System.loadLibrary("RBook");
	}
}
