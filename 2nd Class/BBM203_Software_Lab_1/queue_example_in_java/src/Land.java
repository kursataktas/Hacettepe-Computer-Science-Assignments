

public class Land {
	/* each lands have own queue*/
	protected Queue landQueue = new Queue();             
	protected int noFuelPlane = 0;           // landing successfull but haven't fuel for flying. Not crash
	protected int closedTo = 1;             // land's close time for new plane landing or flying
	protected int stuckOnQueue = 0;        // plane landing time > execution time
	
	protected void setStatus(int closedTo){        // changes land's close time
		this.closedTo = closedTo;
	}
}
