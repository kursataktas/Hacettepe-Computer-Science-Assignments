

public class Queue {
	
	
	Plane firstOfQueue = null, lastOfQueue = null;
	
	public void addQueue(Plane x){                 // adding plane in queue
		
		if(firstOfQueue == null){
			
			firstOfQueue = x;
			firstOfQueue.next = null;
			lastOfQueue = firstOfQueue;
		}
		else{
			
			lastOfQueue.next = x;
			lastOfQueue = lastOfQueue.next;
		}
		
	}
	
	public Plane getQueue(){              // returns plane which place head of queue
		
		if ( firstOfQueue == null)
			return null;
		else{
			
			Plane temp = firstOfQueue;
			firstOfQueue = firstOfQueue.next;
			return temp;
		}
	}
	
	
}
