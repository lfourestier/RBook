package luc.fourestier.rbook;

import java.util.ArrayList;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class BookListAdapter extends ArrayAdapter<String> {
	private final Context context;
	private final ArrayList<String> bookListArray;
	
	public BookListAdapter(Context context, ArrayList<String> booklist) {
		super(context, R.layout.list_item, booklist);
		this.context = context;
		this.bookListArray = booklist;
	}
	
	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		LayoutInflater inflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		
		View rowView = inflater.inflate(R.layout.list_item, parent, false);
		//ImageView imgView = (ImageView) rowView.findViewById(R.id.item_icon);
		TextView titleView = (TextView) rowView.findViewById(R.id.item_title);
		
		// TODO Set a drawable with an image for the icon then set the icon with imgView.setImageDrawable(drawable);
		titleView.setText(bookListArray.get(position));
		
		return rowView;
	}

}
