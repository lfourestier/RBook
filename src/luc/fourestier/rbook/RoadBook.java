package luc.fourestier.rbook;

public class RoadBook {

	/**
	 * Get the title of the road book.
	 * 
	 * @return The title string.
	 */
	public String getBookTitle() {
		return _GetBookName();
	}

	/**
	 * Get the general description of the road book.
	 * 
	 * @return The description string.
	 */
	public String getBookDescription() {
		return _GetBookDescription();
	}

	/**
	 * Get the general location of the road book.
	 * 
	 * @return The location string.
	 */
	public String getBookLocation() {
		return _GetBookLocation();
	}

	/**
	 * Get The total distance
	 * 
	 * @return The distance
	 */
	public float getBookTotalDistance() {
		return _GetBookTotalDistance();
	}

	/**
	 * Get current point Name
	 * 
	 * @return The string
	 */
	public String getCurrentPointName() {
		return _GetCurrentPointName();
	}

	/**
	 * Get current point RoadDetails
	 * 
	 * @return The string
	 */
	public String getCurrentPointObservation() {
		return _GetCurrentPointObservation();
	}

	/**
	 * Get current point Kilometer
	 * 
	 * @return The float
	 */
	public float getCurrentPointKilometer() {
		return _GetCurrentPointKilometer();
	}

	/**
	 * Get current point Type
	 * 
	 * @return The string
	 */
	public String getCurrentPointType() {
		return _GetCurrentPointType();
	}

	/**
	 * Get current point Direction
	 * 
	 * @return The string
	 */
	public String getCurrentPointDirection() {
		return _GetCurrentPointDirection();
	}

	/**
	 * Get current point DistanceToNext
	 * 
	 * @return The float
	 */
	public float getCurrentPointDistanceToNext() {
		return _GetCurrentPointDistanceToNext();
	}

	/**
	 * Get previous point DistanceToNext
	 * 
	 * @return The float
	 */
	public float getCurrentPointDistanceFromPrevious() {
		return _GetCurrentPointDistanceFromPrevious();
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

	// protected
	
	
	protected int nativeInstance;
	
	protected RoadBook() {
	}

	// Private

	private static final String TAG = RoadBook.class.getSimpleName();

	// Native

	private native final String _GetBookName();

	private native final String _GetBookDescription();

	private native final String _GetBookLocation();

	private native final float _GetBookTotalDistance();

	private native final String _GetCurrentPointName();

	private native final String _GetCurrentPointObservation();

	private native final float _GetCurrentPointKilometer();

	private native final String _GetCurrentPointType();

	private native final String _GetCurrentPointDirection();

	private native final float _GetCurrentPointDistanceToNext();

	private native final float _GetCurrentPointDistanceFromPrevious();

	private native final void _Next();

	private native final void _Previous();

	private native final void _Reset();
}
