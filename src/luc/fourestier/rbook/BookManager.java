package luc.fourestier.rbook;

import java.util.ArrayList;

public class BookManager {

	/**
	 * Create a book manager and initialize it with rootdir.
	 * 
	 * @param rootdir: Where to store roadbooks.
	 * @return The instance of the book manager.
	 * @exception java/lang/RuntimeException: any internal native resource errors; 
	 * @exception java/io/IOException: any file/dir errors; 
	 */
	public static BookManager Create(String rootdir) {
		BookManager bm = new BookManager();
		bm._Create(rootdir);
		return bm;
	}
	
	/**
	 * Get the list of roadbook available in the RBook directory.
	 * @return List of books
	 */
	public ArrayList<String> getRoadBookList() {
		String serializedlist = _GetRoadBookListSerialized();
		String[] list = serializedlist.split(";");
		ArrayList<String> retlist = new ArrayList<String>();
		
	    for (int i = 0; i < list.length; ++i) {
	    	retlist.add(list[i]);
	      }

		return retlist;
	}
	
	/**
	 * Get a road book thanks to its bookname
	 * 
	 * @param bookname: the bookname (ie: simple file name w/o extension)
	 * @return The road book
	 * @exception java/lang/RuntimeException: any internal native resource errors; 
	 * @exception java/io/IOException: any file errors; 
	 * @exception java/lang/IndexOutOfBoundsException: Book list error, empty or book not found; 
	 */
	public RoadBook getRoadBook(String bookname) {
		RoadBook rb = new RoadBook();
		rb.nativeInstance = _GetRoadBook(bookname);
		
		return rb;
	}
	
	/**
	 * Create a new blank road book.
	 * 
	 * @param bookname: the bookname (ie: simple file name w/o extension)
	 * @return The road book
	 * @exception java/lang/RuntimeException: any internal native resource errors; 
	 */
	public RoadBook createRoadBook(String bookname) {
		RoadBook rb = new RoadBook();
		rb.nativeInstance = _CreateRoadBook(bookname);
		
		return rb;
		
	}
	
	/**
	 * Save the road book.
	 * 
	 * @param roadbook: the roadbook to save.
	 * @exception java/lang/RuntimeException: any internal native resource errors; 
	 * @exception java/io/IOException: any file errors; 
	 */
	public void saveRoadBook(RoadBook roadbook) {
		_SaveRoadBook(roadbook.nativeInstance);
	}
	 
	/**
	 * Release the road book.
	 * The user MUST release the roadbook native resources by calling that function.
	 * Once used, Roadbook is not usable anymore and user must call GetRoadBook to get it again.
	 * 
	 * @param roadbook: the roadbook to release.
	 * @exception java/lang/RuntimeException: any internal native resource errors; 
	 * @exception java/io/IOException: any file errors; 
	 * @exception java/lang/IndexOutOfBoundsException: Book list error, empty or book not found; 
	 */
	public void releaseRoadBook(RoadBook roadbook) {
		_ReleaseRoadBook(roadbook.nativeInstance);
		roadbook.nativeInstance = 0;
	}
	 
	/**
	 * Delete the roadbook.
	 * 
	 * @param bookname: the roadbook to delete.
	 * @exception java/lang/RuntimeException: any internal native resource errors; 
	 * @exception java/io/IOException: any file errors; 
	 */
	public void deleteRoadBook(String bookname) {
		_DeleteRoadBook(bookname);
	}
	
	/**
	 * Releases the native BookManager instance and all resources associated with it.
	 * 
	 * @exception java/lang/RuntimeException: any internal native resource errors; 
	 */
	public void release() {
		_Release();
	}

// Private
    
    private int nativeInstance;
    private static final String TAG = BookManager.class.getSimpleName();

    private BookManager() {}
    
// Native 

	private native final void _Create(String rootdir);
	private native final String _GetRoadBookListSerialized();
	private native final int _GetRoadBook(String bookname);
	private native final int _CreateRoadBook(String bookname);
	private native final void _SaveRoadBook(int instance);
	private native final void _ReleaseRoadBook(int instance);
	private native final void _DeleteRoadBook(String bookname);
	private native final void _Release();
}
