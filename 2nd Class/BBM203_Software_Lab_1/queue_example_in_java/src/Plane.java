

public class Plane {
	
	private int id;                            // plane unique id
	protected String name;                    // plane name
	protected int fuel;                      
	protected Plane next;				    // reference to next plane on queue	
	protected int time;                    // first time 
	protected int arrivingTime;			  // landing time
	protected int departingTime;
	protected int waitingTime = 0;		// waiting time for landing or flying 
	
	public Plane(int time, String name,int fuel){
		
		this.name = name;
		this.fuel = fuel;
		this.time = time;
		this.id = (int)(Math.random() * 88888888);
	}
	
	public int getId(){
		
		return this.id;
	}
	
	public boolean isCrash(int time,int consumption){         // return true if plane has crash
		
		if ( (this.fuel = fuel - (time) * consumption) >= 0 ){
			
			this.waitingTime = 0;
			return false;
		}
			
		else
			return true;
	}
}
