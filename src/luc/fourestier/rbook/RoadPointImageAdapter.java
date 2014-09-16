package luc.fourestier.rbook;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageView;

public class RoadPointImageAdapter extends BaseAdapter {
	    private Context mContext;
		private final String drawablePrefix = "drawable";

	    public RoadPointImageAdapter(Context c) {
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
	        
	        int id = 0;
	        if (mPointNames != null) {
			id = mContext.getResources().getIdentifier(mPointNames[position], drawablePrefix, mContext.getPackageName());
	        }
	        
			if (id != 0) {
				imageView.setImageResource(id);
			} else {
				imageView.setImageResource(R.drawable.question_mark);
			}
	        return imageView;
	    }

	    // references to our images
	    public String[] mPointNames = null;
	}