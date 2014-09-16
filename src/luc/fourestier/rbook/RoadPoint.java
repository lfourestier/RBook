package luc.fourestier.rbook;

public class RoadPoint {

	/**
	 * Get point description.
	 * 
	 * @return The string.
	 */
	public String getDescription() {
		return _GetDescription();
	}

	/**
	 * Set point description.
	 * 
	 * @param description: the description.
	 */
	public void setDescription(String description) {
		_SetDescription(description);
	}

	/**
	 * Get point Kilometer.
	 * 
	 * @return The float.
	 */
	public float getKilometer() {
		return _GetKilometer();
	}

	/**
	 * Set point kilometer.
	 * 
	 * @param kilometer: the kilometer.
	 */
	public void setKilometer(float kilometer) {
		_SetKilometer(kilometer);
	}

	/**
	 * Get point Type.
	 * 
	 * @return The string.
	 */
	public String getType() {
		return _GetType();
	}

	/**
	 * Set point type.
	 * 
	 * @param type: the type.
	 */
	public void setType(String type) {
		_SetType(type);
	}

	/**
	 * Get point Direction.
	 * 
	 * @return The string.
	 */
	public String getDirection() {
		return _GetDirection();
	}

	/**
	 * Set point direction.
	 * 
	 * @param direction: The direction.
	 */
	public void setDirection(String direction) {
		_SetDirection(direction);
	}
	
	/**
	 * Get the point number in the list.
	 * 
	 * @return The number.
	 */
	public int getNumber() {
		return _GetNumber();
	}

	// protected
	
	protected int nativeInstance;
	
	protected RoadPoint() {
	}

	// Private

	private static final String TAG = RoadPoint.class.getSimpleName();

	// native
	
	private native final String _GetDescription();

	private native final void _SetDescription(String description);
	
	private native final float _GetKilometer();

	private native final void _SetKilometer(float kilometer);
	
	private native final String _GetType();

	private native final void _SetType(String type);
	
	private native final String _GetDirection();
	
	private native final void _SetDirection(String direction);
	
	private native final int _GetNumber();
}
