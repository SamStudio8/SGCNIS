package co.uk.studio8media.uni.cs225;

import java.io.File;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Data Entry application's loading class.
 * 
 * @author Sam Nicholls (msn)
 */
public class Main {
	
	private static Writer writer = new Writer("");
	private static Console console = new Console();

	/**
	 * The application launcher.
	 * @param args Command line arguments.
	 */
	public static void main(String[] args) {
		
		log("[APP] The Data Entry application was launched.");
		
		char mode = 0;
		while(mode != 'q' && mode != 'Q'){
			System.out.println("            ,:',:`,:'");
			System.out.println("         __||_||_||_||__");
			System.out.println("    ____[\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"]____");
			System.out.println("    \\ \" '''''''''''''''''''' \\");
			System.out.println("  ~~^~^~^~^~^^~^~^~^~^~^~^~^~~^~^~^^~~^~^");
			System.out.println("**************************************************");
			System.out.println("St. George's Channel Navigation Information System");
			System.out.println("                Data Entry Module");
			System.out.println("**************************************************");
			System.out.println("A  Add or Update Ship");
			System.out.println("D  Delete Ship");
			System.out.println("W  Change Working Directory");
			System.out.println("Q  Quit");
			mode = console.prompt("\nPlease select an option", "\\w").charAt(0);
			
			switch(mode){
				case 'A':
				case 'a':
					addOrUpdateShip();
					break;
				case 'D':
				case 'd':
					deleteShip(console.prompt("MMSI", "\\d{9}"));
					break;
				case 'W':
				case 'w':
					editWorkingDirectory();
					break;
			}
		}
		log("[APP] The Data Entry application was closed.");
	}
		
	/**
	 * Method called from application menu to prompt the user for the
	 * information required to add or update a ship to the system.
	 * The method then attempts to write the information to a 
	 * ship data file and documents the result in the log file.
	 */
	private static void addOrUpdateShip(){
		double lng = -360.0;
		double lat = -360.0;
		int course = -1;
		double speed = -1;
		
		String mmsi = console.prompt("MMSI (9 Digit ID)", "\\d{9}");
		String name = console.prompt("Name", "[\\w+ ]+");
		
		while(lng < -180 || lng > 180){
			lng = Double.parseDouble(console.prompt("Longitude (-180.000 - 180.000)", "-?\\d+\\.\\d{3}"));
		}
		
		while(lat < -90 || lat > 90){
			lat = Double.parseDouble(console.prompt("Latitude (-90.000 - 90.000)", "-?\\d+\\.\\d{3}"));
		}
		
		while(course < 0 || course > 360){
			course = Integer.parseInt(console.prompt("Course (Degs clockwise from N, 0 - 360)", "-?\\d+"));
		}
		
		while(speed < 0){
			speed = Double.parseDouble(console.prompt("Speed (Knots, 1 dp)", "\\d+\\.\\d"));
		}
		
		DateFormat dateFormat = new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss Z");
		Date date = new Date();
			
		StringBuffer sb = new StringBuffer();
		sb.append(mmsi+"\n");
		sb.append(name+"\n");
		sb.append(lat+" ");
		sb.append(lng+" ");
		sb.append(course+" ");
		sb.append(speed+"\n");
		sb.append(dateFormat.format(date));
			
		//Attempt to write to the ship data file.
		if((writer.write(mmsi+".txt", sb.toString(), 0)) == 1){
			displayAndLog("[NEW] "+mmsi+".txt was added to (or updated in) the working directory.");
		}
		else{
			displayAndLog("[ERR] "+mmsi+".txt was not added to (or updated in) the working directory.");
		}
		
		char option = 0;
		while(option != 'a' && option != 'A' && option != 'x' && option != 'X'){
			option = console.prompt("\n(A) Add or Update Another Ship, (X) Return to Main Menu", "\\w").charAt(0);
		}
		switch(option){
			case 'A':
			case 'a':
				addOrUpdateShip();
				break;
			case 'X':
			case 'x':
				break;
		}	
	}
	
	/**
	 * Method called from the application menu to enable a user to delete
	 * a ship that has previously been entered in to the system.
	 * 
	 * @param mmsi The MMSI of the ship to remove from the system.
	 */
	private static void deleteShip(String mmsi){		
		File file = new File(writer.getPwd()+mmsi+".txt");
		if(file.delete()){
			displayAndLog("[DEL] "+mmsi+".txt was deleted from the working directory.");			
		}
		else{
			displayAndLog("[ERR] "+mmsi+".txt could not be deleted from the working directory.");	
		}
		
		char option = 0;
		while(option != 'd' && option != 'D' && option != 'x' && option != 'X'){
			option = console.prompt("\n(D) Delete Another Ship, (X) Return to Main Menu", "\\w").charAt(0);
		}
		switch(option){
			case 'D':
			case 'd':
				deleteShip(console.prompt("MMSI", "\\d{9}"));
				break;
			case 'X':
			case 'x':
				break;
		}
	}
	
	/**
	 * Simultaneously display and log a piece of information or message 
	 * the application has produced to the system log file.
	 * 
	 * @param line The string to display to the user and write to the log.
	 */
	private static void displayAndLog(String line){
		System.out.println(line);
		writer.write("log.txt", line, 1);
	}
	
	/**
	 * Log a piece of information or message from the application to the log file.
	 * 
	 * @param line The string to write to the log.
	 */
	private static void log(String line){
		writer.write("log.txt", line, 1);
	}

	/**
	 * Method called from the application menu to allow the user to
	 * change the path of the working directory in which the application 
	 * writes its ship files and log entries.
	 */
	private static void editWorkingDirectory(){
		System.out.println("The current working directory is: \""+writer.getPwd()+"\"");
		char option = 0;
		while(option != 'y' && option != 'Y' && option != 'n' && option != 'N'){
			option = console.prompt("Do you wish to change the present working directory? [Y/N]", "\\w").charAt(0);
		}
		
		switch(option){
		case 'Y':
		case 'y':
			String newPwd = console.prompt("New Working Directory (Include Final Directory Separator)", "[\\w+\\.: \\\\/]*");
			log("[PWD] The Data Entry application's PWD was changed to "+newPwd);
			log("[PWD] The Data Entry application log was changed to "+newPwd+"log.txt");
			writer = new Writer(newPwd);
			System.out.println("\nNOTE! For security, when the location of the log file is changed, a notice is printed to the original log.\nIf this file did not exist, two warnings above will display and can be safely disregarded.\n");
			displayAndLog("[PWD] The Data Entry application's PWD was changed to "+newPwd);
			displayAndLog("[PWD] The Data Entry application log was changed to "+newPwd+"log.txt");
			break;
		case 'N':
		case 'n':
			break;
		}
	}
}
