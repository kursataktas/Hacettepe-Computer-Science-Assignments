
import java.io.*;

public class Main {
	
	// arguments
	protected static String inputFilePath, outputFilePath, queueType;
	protected static int executionTime, fuelConsumption, landingTime, takeOffTime, totalArrivedPlane = 0, numberOfCrash = 0;   // totalArrivedPlane -> all planes in system which not crashed
	
	// file 
	protected static BufferedReader inputReader=null;                 
	protected static BufferedWriter outputWriter=null;
	
	// lands and queues
	protected static Land runway0 = new Land();                        // arriving land 1
	protected static Land runway1 = new Land();                       // arriving land 2
	protected static Land runway2 = new Land();						 // takeoff land
	
	public static void main(String []args){
			
		try {
			
			getParameters(args); 						// gets main parameters
			getInputFile(inputFilePath);               // assigns inputfileReader
			getOutputFile(outputFilePath);			  // assigns outputFileReader
			setArrivingQueues();				     // from inputFile to land 1 and land 2 queues
			setDepartingQueues();					// from land 1 , land 2 queues to takeoffqueues
			flyPlanes();						   // flyies planes which in the takeoff queue
			setOutputFile();					  // writes results to output file
			
		}
		catch(IOException E){                       // catch ioexceptions
			
			System.err.println(E.getMessage());
		}
		catch (Exception e) {                       // catch all exceptions
			System.err.println(e.getMessage());
		}
		finally{                                    // finally must close all files
			
			if(outputWriter!=null){
				try {
					outputWriter.close();
				} catch (Exception e2) {
					System.err.println(e2.getMessage());
				}
			}
			if(inputReader!=null){
				try {
					inputReader.close();
				} catch (Exception e2) {
					System.err.println(e2.getMessage());
				}
			}
		}
		
	}
	
	private static void flyPlanes(){
		
		while ( runway2.landQueue.firstOfQueue != runway2.landQueue.lastOfQueue.next){           // fly planes which in takeoff queue
			
			totalArrivedPlane++;																// all planes that haven't chrashed
			if ( runway2.landQueue.firstOfQueue.time +  runway2.landQueue.firstOfQueue.waitingTime >= executionTime){         // controlling execution time
				runway2.landQueue.getQueue(); 																				// getting plane from queue
				runway2.stuckOnQueue++;																					   // over execution time. planes stucks on queue
			}
				
			else if ( runway2.landQueue.firstOfQueue.isCrash(takeOffTime + runway2.landQueue.firstOfQueue.waitingTime , fuelConsumption)){   // controlling crash
				runway2.landQueue.getQueue();
				numberOfCrash++;
			}
			else{
				runway2.landQueue.getQueue();														// flying planes
			}
		}
	}
	private static void setArrivingQueues() throws NumberFormatException, IOException{
		
		String commands;
		while( (commands = inputReader.readLine()) != null){			// reads inputs from input file
			
			String[] splitedCommands = commands.split("\\s+");         // splits inputs by spaces
			
			Plane newPlane = new Plane(Integer.parseInt(splitedCommands[0]),splitedCommands[1], Integer.parseInt(splitedCommands[2]));   // creates planes object
			
			if(executionTime > newPlane.time){               // controlling execution time
				
				if (newPlane.time > runway0.closedTo){		// which land available for landing ?
					
					newPlane.arrivingTime = newPlane.time + landingTime;
					runway0.landQueue.addQueue(newPlane);
					runway0.setStatus(newPlane.time + landingTime);
					
				}
				else if (newPlane.time > runway1.closedTo){
					
					newPlane.arrivingTime = newPlane.time + landingTime;
					runway1.landQueue.addQueue(newPlane);
					runway1.setStatus(newPlane.time + landingTime);
				}
				else{															// if two land full. Then place plane to queue with waiting time ( kind a reservation )
					
					if (runway0.closedTo <= runway1.closedTo){										
						
						newPlane.waitingTime = runway0.closedTo - newPlane.time;						// plane's waiting time.
						newPlane.arrivingTime = newPlane.time + newPlane.waitingTime + landingTime;		// plane's landing time.
						runway0.landQueue.addQueue(newPlane);											// add land's queue
						runway0.setStatus(newPlane.time + newPlane.waitingTime + landingTime);			// set land's status ( close time )
						
						
					}
					else{
						
						newPlane.waitingTime = runway1.closedTo - newPlane.time;
						newPlane.arrivingTime = newPlane.time + newPlane.waitingTime + landingTime;
						runway1.landQueue.addQueue(newPlane);
						runway1.setStatus(newPlane.time + newPlane.waitingTime + landingTime);
						
					}
				}
				
			}
			else{
				return;
			}
		}
	}
	private static void getParameters(String []args) throws Exception{
		
		inputFilePath = args[0];
		outputFilePath = args[1];
		executionTime = Integer.parseInt(args[2]);
		fuelConsumption = Integer.parseInt(args[3]);
		landingTime = Integer.parseInt(args[4]);
		takeOffTime = Integer.parseInt(args[5]);
		queueType = args[6];
		
	}
	private static void getInputFile(String path) throws IOException{
		inputReader = new BufferedReader(new FileReader(new File(inputFilePath)));
	}
	private static void getOutputFile(String path) throws IOException{
		outputWriter = new BufferedWriter(new FileWriter(new File(outputFilePath)));
	}
	private static void setOutputFile() throws IOException{
		
		totalArrivedPlane += runway0.noFuelPlane + runway1.noFuelPlane;
		
		outputWriter.write("Number of planes which are in the landing queue 1 : " + runway0.stuckOnQueue);		outputWriter.newLine();
		outputWriter.write("Number of planes which are in the landing queue 2 : " + runway1.stuckOnQueue);		outputWriter.newLine();
		outputWriter.write("Number of planes which are in the takeoff queue : " + runway2.stuckOnQueue);		outputWriter.newLine();
		outputWriter.write("Total number of planes arrived : " + totalArrivedPlane);		outputWriter.newLine();
		outputWriter.write("Total number of planes that have crashed : " + numberOfCrash);		
	}
	private static void setDepartingQueues() throws Exception{
		
		while(true){			
			
			if( runway0.landQueue.firstOfQueue != null && runway1.landQueue.firstOfQueue != null){	   // if two landing queues has at least one plane
				
				if ( runway0.landQueue.firstOfQueue.arrivingTime <= runway1.landQueue.firstOfQueue.arrivingTime){   // compare plane's time from landing runways
					
					if ( runway0.landQueue.firstOfQueue.arrivingTime < executionTime ){	
					
						if ( runway0.landQueue.firstOfQueue.isCrash(landingTime + runway0.landQueue.firstOfQueue.waitingTime, fuelConsumption) ){
							
							runway0.landQueue.getQueue();
							numberOfCrash++; 
						}
						else{
							
							if (runway0.landQueue.firstOfQueue.fuel == 0){				// here is the planes which landing successfully but has not enough fuel for flying
								
								runway0.landQueue.getQueue();
								runway0.noFuelPlane++;
							}
							else{
								
								if (runway0.landQueue.firstOfQueue.arrivingTime  < runway2.closedTo){			// adding waiting time plane's ( kind a reservation )
									runway0.landQueue.firstOfQueue.waitingTime = runway2.closedTo - runway0.landQueue.firstOfQueue.arrivingTime;
								}
								runway2.closedTo = runway0.landQueue.firstOfQueue.arrivingTime + runway0.landQueue.firstOfQueue.waitingTime + takeOffTime;    // sets departing land's close time
								runway2.landQueue.addQueue(runway0.landQueue.getQueue());
							}
						}
					}
					else{
						
						runway0.landQueue.getQueue();               
						runway0.stuckOnQueue++;
					}
				}
				else{
					
					if ( runway1.landQueue.firstOfQueue.arrivingTime < executionTime ){
						
						if ( runway1.landQueue.firstOfQueue.isCrash(landingTime + runway1.landQueue.firstOfQueue.waitingTime, fuelConsumption) ){
							
							runway1.landQueue.getQueue();
							numberOfCrash++; 
						}
						else{
							
							if (runway1.landQueue.firstOfQueue.fuel == 0){
								
								runway1.landQueue.getQueue();
								runway1.noFuelPlane++;
							}
							else{
								
								if (runway1.landQueue.firstOfQueue.arrivingTime  < runway2.closedTo){
									runway1.landQueue.firstOfQueue.waitingTime = runway2.closedTo - runway1.landQueue.firstOfQueue.arrivingTime;
								}
								runway2.closedTo = runway1.landQueue.firstOfQueue.arrivingTime + runway1.landQueue.firstOfQueue.waitingTime + takeOffTime;
								runway2.landQueue.addQueue(runway1.landQueue.getQueue());
							}
						}
					}
					else{
						
						runway1.landQueue.getQueue();
						runway1.stuckOnQueue++;
					}
				}
			}
			
			else if ( runway0.landQueue.firstOfQueue == null && runway1.landQueue.firstOfQueue != null ){
						
				if ( runway1.landQueue.firstOfQueue.arrivingTime < executionTime ){
					
					if ( runway1.landQueue.firstOfQueue.isCrash(landingTime + runway1.landQueue.firstOfQueue.waitingTime, fuelConsumption) ){
						
						runway1.landQueue.getQueue();
						numberOfCrash++; 
					}
					else{
						
						if (runway1.landQueue.firstOfQueue.fuel == 0){
							
							runway1.landQueue.getQueue();
							runway1.noFuelPlane++;
						}
						else{
							
							if (runway1.landQueue.firstOfQueue.arrivingTime  < runway2.closedTo){
								runway1.landQueue.firstOfQueue.waitingTime = runway2.closedTo - runway1.landQueue.firstOfQueue.arrivingTime;
							}
							runway2.closedTo = runway1.landQueue.firstOfQueue.arrivingTime + runway1.landQueue.firstOfQueue.waitingTime + takeOffTime;
							runway2.landQueue.addQueue(runway1.landQueue.getQueue());
						}
					}
				}
				else{
					
					runway1.landQueue.getQueue();
					runway1.stuckOnQueue++;
				}
			}
			else if( runway1.landQueue.firstOfQueue == null && runway0.landQueue.firstOfQueue != null ){
							
				if ( runway0.landQueue.firstOfQueue.arrivingTime < executionTime ){
					
					if ( runway0.landQueue.firstOfQueue.isCrash(landingTime + runway0.landQueue.firstOfQueue.waitingTime, fuelConsumption) ){
						
						runway0.landQueue.getQueue();
						numberOfCrash++; 
					}
					else{
						
						if (runway0.landQueue.firstOfQueue.fuel == 0){
							
							runway0.landQueue.getQueue();
							runway0.noFuelPlane++;
						}
						else{
							
							if (runway0.landQueue.firstOfQueue.arrivingTime  < runway2.closedTo){
								runway0.landQueue.firstOfQueue.waitingTime = runway2.closedTo - runway0.landQueue.firstOfQueue.arrivingTime;
							}
							runway2.closedTo = runway0.landQueue.firstOfQueue.arrivingTime + runway0.landQueue.firstOfQueue.waitingTime + takeOffTime;
							runway2.landQueue.addQueue(runway0.landQueue.getQueue());
						}
					}
				}
				else{
					
					runway0.landQueue.getQueue();
					runway0.stuckOnQueue++;
				}
			}
			
			else if ( runway0.landQueue.firstOfQueue == null && runway1.landQueue.firstOfQueue == null )
				break;
			
		}
	}
}
