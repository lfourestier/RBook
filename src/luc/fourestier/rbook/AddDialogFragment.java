package luc.fourestier.rbook;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.view.LayoutInflater;
import android.widget.EditText;



public class AddDialogFragment extends DialogFragment {
	public String roadBookName;
    
    public interface AddDialogListener {
        public void onDialogPositiveClick(DialogFragment dialog);
    }
    
    AddDialogListener listener;
    
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        try {
            listener = (AddDialogListener) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString() + " must implement NoticeDialogListener");
        }
    }
    
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflater = getActivity().getLayoutInflater();
        
        builder.setView(inflater.inflate(R.layout.dialog_add_book, null));
        
        builder.setPositiveButton(R.string.button_ok, new DialogInterface.OnClickListener() {
                   public void onClick(DialogInterface dialog, int id) {
                	   Dialog d = (Dialog) dialog;
                	   EditText nameText = (EditText) d.findViewById(R.id.add_book_text);
                	   roadBookName = nameText.getText().toString();
                       listener.onDialogPositiveClick(AddDialogFragment.this);
                       AddDialogFragment.this.getDialog().dismiss();
                   }
               })
               .setNegativeButton(R.string.button_cancel, new DialogInterface.OnClickListener() {
                   public void onClick(DialogInterface dialog, int id) {
                       AddDialogFragment.this.getDialog().cancel();
                   }
               });
        return builder.create();
    }

}
