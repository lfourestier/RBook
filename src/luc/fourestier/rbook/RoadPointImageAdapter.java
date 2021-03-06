package luc.fourestier.rbook;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageView;

public class RoadPointImageAdapter extends BaseAdapter {
	    private Context mContext;
		
		private PictManager thePictManager = null;

	    public RoadPointImageAdapter(Context c) {
	    	thePictManager = MainActivity.thePictManager;
	        mContext = c;
	    }

	    public int getCount() {
	    	int count = 0;
	    	if (mPointNames != null) {
	    		count = mPointNames.length;
	    	}
	        return count;
	    }

	    public Object getItem(int position) {
	        return null;
	    }

	    public long getItemId(int position) {
	        return 0;
	    }

	    // create a new ImageView for each item referenced by the Adapter
	    public View getView(int position, View convertView, ViewGroup parent) {
	        ImageView imageView;
	        if (convertView == null) {  // if it's not recycled, initialize some attributes
	            imageView = new ImageView(mContext);
	            imageView.setLayoutParams(new GridView.LayoutParams(85, 85));
	            imageView.setScaleType(ImageView.ScaleType.CENTER_CROP);
	            imageView.setPadding(8, 8, 8, 8);
	        } else {
	            imageView = (ImageView) convertView;
	        }
	        
	        if ((mPointNames != null) && (mPointNames.length > 0)) {
	        	try {
		        	String path = thePictManager.getPict(mPointNames[position], thePictManager.RESOLUTION_SMALL);
	
		            Bitmap bitmap = BitmapFactory.decodeFile(path);
		            imageView.setImageBitmap(bitmap);
	        	}
	        	catch (IndexOutOfBoundsException e) {
	        		imageView.setImageResource(R.drawable.question_mark_small);
	        	}
	        }
	        else {
				imageView.setImageResource(R.drawable.question_mark_full);
	        }
	        
	        return imageView;
	    }

	    // references to our images
	    public String[] mPointNames = null;
	}