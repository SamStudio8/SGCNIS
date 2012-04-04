package co.uk.studio8media.uni.cs225;

import java.util.Scanner;

/**
 * Provides a shared prompt method to components of the Data Entry application.
 * 
 * @author Sam Nicholls (msn)
 */
public class Console {

	/**
	 * Method to prompt a user to provide a valid input.
	 * The user's response is validated against a regular expression.
	 * The method will prompt the user for input again if the pattern did not match.
	 * 
	 * @param promptFor A string representing the expected input from the user
	 * @param parseFor A string regular expression to use to validate input
	 * @return The user's valid response
	 */
	public String prompt(String promptFor, String parseFor){
		Scanner scan = new Scanner(System.in).useDelimiter("\n");
		String output = "";
		
		while(output.equals("")){
			System.out.print(promptFor+": ");
			if(scan.hasNext(parseFor)){
				output = scan.next(parseFor);
			}
			else{
				scan.next();
			}
		}
		return output;
	}
}
