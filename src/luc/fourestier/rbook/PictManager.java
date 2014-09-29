package luc.fourestier.rbook;

import java.util.ArrayList;

import android.util.Log;

public class PictManager {
	
	/*
	 * DO NOT change unless you changed Resolution enum in RBook/PictManager.h 
	 */
	public final int RESOLUTION_FULL = 0; 
	public final int RESOLUTION_SMALL = 1; 

	/**
	 * Create a Pictmanager and populate the picts.
	 * @param archive: Archive of pict to inflate.
	 * @param rootdir: Directory where to inflate.
	 * @return The PictManager.
	 */
	public static PictManager create(String archive, String rootdir) {
		PictManager pictmanager = new PictManager();
		pictmanager._Populate(archive, rootdir);
		return pictmanager;
	}
	
	/**
	 * Get the list of available picts.
	 * @return The list of pict paths.
	 */
	public ArrayList<String> getPictList() {
		String serializedlist = _GetPictListSerialized();
		String[] list = serializedlist.split(";");
		ArrayList<String> retlist = new ArrayList<String>();
		
	    for (int i = 0; i < list.length; ++i) {
	    	retlist.add(list[i]);
	      }

		return retlist;
	}
	
	/**
	 * Get a pict.
	 * @param pict: The pict name to get.
	 * @param resolution: the request resolution.
	 * @return
	 */
	public String getPict(String pict, int resolution) {
		String	path = _GetPict(pict, resolution);
		
		return path;
	}
	
// Private
    
    private int nativeInstance;
    private static final String TAG = PictManager.class.getSimpleName();

    private PictManager() {}
    
// Native 

	private native final void _Populate(String archive, String rootdir);
	private native final String _GetPictListSerialized();
	private native final String _GetPict(String pict, int resolution);

}
