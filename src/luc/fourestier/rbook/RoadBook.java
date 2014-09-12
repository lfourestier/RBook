package luc.fourestier.rbook;

public class RoadBook {
	
	/**
	 * Get the bookname (Short name).
	 * 
	 * @return The bookname.
	 */
	public String getBookName() {
		return _GetBookName();
	}

	/**
	 * Get the title of the road book.
	 * 
	 * @return The title string.
	 */
	public String getTitle() {
		return _GetTitle();
	}

	/**
	 * Set title.
	 * 
	 * @param title: the title.
	 */
	public void setTitle(String title) {
		_SetTitle(title);
	}

	/**
	 * Get the general description of the road book.
	 * 
	 * @return The description string.
	 */
	public String getDescription() {
		return _GetDescription();
	}

	/**
	 * Set description.
	 * 
	 * @param description: the description.
	 */
	public void setDescription(String description) {
		_SetDescription(description);
	}

	/**
	 * Get the general location of the road book.
	 * 
	 * @return The location string.
	 */
	public String getLocation() {
		return _GetLocation();
	}

	/**
	 * Set location.
	 * 
	 * @param location: the location.
	 */
	public void setLocation(String location) {
		_SetLocation(location);
	}

	/**
	 * Get The total distance
	 * 
	 * @return The distance
	 */
	public float getTotalDistance() {
		return _GetTotalDistance();
	}
	
	/**
	 * Get the current road point.
	 * @return The current road point.
	 */
	public RoadPoint getCurrentPoint() {
		RoadPoint roadpoint = new RoadPoint();
		roadpoint.nativeInstance = _GetCurrentPoint();
		
		return roadpoint;
	}

	/**
	 * Get the previous road point.
	 * @return The previous road point.
	 */
	public RoadPoint getPreviousPoint() {
		RoadPoint roadpoint = new RoadPoint();
		roadpoint.nativeInstance = _GetPreviousPoint();
		
		return roadpoint;
	}

	/**
	 * Get the next road point.
	 * @return The next road point.
	 */
	public RoadPoint getNextPoint() {
		RoadPoint roadpoint = new RoadPoint();
		roadpoint.nativeInstance = _GetNextPoint();
		
		return roadpoint;
	}

	/**
	 * Get distance to next point in the list
	 * 
	 * @return The float distance
	 */
	public float getDistanceToNext() {
		return _GetDistanceToNext();
	}

	/**
	 * Get distance from previous point in the list
	 * 
	 * @return The float distance
	 */
	public float getDistanceFromPrevious() {
		return _GetDistanceFromPrevious();
	}

	/**
	 * Add a new road point before the current position.
	 * @return The new created empty roadpoint.
	 */
	public RoadPoint addNewPointBefore() {
		RoadPoint roadpoint = new RoadPoint();
		roadpoint.nativeInstance = _AddNewPointBefore();
		
		return roadpoint;
	}

	/**
	 * Add a new road point after the current position.
	 * @return The new created empty roadpoint.
	 */
	public RoadPoint addNewPointAfter() {
		RoadPoint roadpoint = new RoadPoint();
		roadpoint.nativeInstance = _AddNewPointAfter();
		
		return roadpoint;
	}

	/**
	 * Advance to next road point.
	 */
	public void next() {
		_Next();
	}

	/**
	 * Go back to previous road point.
	 */
	public void previous() {
		_Previous();
	}

	/**
	 * Reset to the first road point.
	 */
	public void reset() {
		_Reset();
	}
	
	/**
	 * Go to last road point
	 */
	public void end() {
		_End();
	}

	// protected
	
	
	protected int nativeInstance;
	
	protected RoadBook() {
	}

	// Private

	private static final String TAG = RoadBook.class.getSimpleName();

	// Native

	private native final String _GetBookName();
	
	private native final String _GetTitle();

	private native final void _SetTitle(String title);
	
	private native final String _GetDescription();

	private native final void _SetDescription(String description);
	
	private native final String _GetLocation();

	private native final void _SetLocation(String location);
	
	private native final float _GetTotalDistance();

	private native final int _GetCurrentPoint();
	
	private native final int _GetPreviousPoint();
	
	private native final int _GetNextPoint();
	
	private native final float _GetDistanceToNext();

	private native final float _GetDistanceFromPrevious();

	private native final int _AddNewPointBefore();
	
	private native final int _AddNewPointAfter();
	
	private native final void _Next();

	private native final void _Previous();

	private native final void _Reset();
	
	private native final void _End();
}
