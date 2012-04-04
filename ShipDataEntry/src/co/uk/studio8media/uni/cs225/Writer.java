package co.uk.studio8media.uni.cs225;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.channels.FileLock;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * A mechanism, capable of file locking and release for which to write
 * to the ship data files and the application's log file.
 * 
 * @author Sam Nicholls (msn)
 */
public class Writer {
	
	private String pwd;
	private FileOutputStream fos;
	private DateFormat dateFormat = new SimpleDateFormat("d/MM/yy HH:mm:ss");
	private Console console = new Console();
	
	/**
	 * Writer constructor that assigns the FileOutputStream.
	 * @param pwd The present working directory.
	 */
	public Writer(String pwd){
		this.pwd = pwd;
		
		try {
			fos = new FileOutputStream(pwd+"log.txt", true);
		} catch (FileNotFoundException e) {
			System.out.println("WARNING! The log file could not be found.\nAll actions will go un-logged until this is rectified.\nYou may need to change the PWD with the 'W' command on the main menu.");
		}
	}
	
	/**
	 * Write to a file.
	 * 
	 * @param filename The filename to write to.
	 * @param line The line or lines to write to the file.
	 * @param mode Dictates certain format attributes based on whether
	 *  the output is to a log file (1) or ship file (0)
	 *  
	 * @return An int which represents whether the write failed (0) or
	 * was succesfully executed (1).
	 */
	public int write(String filename, String line, int mode){
		int status = 0;
		
    	try {
    		if(mode == 1){
    			fos = new FileOutputStream(pwd+filename, true);
    		}
    		else{
    			fos = new FileOutputStream(pwd+filename, false);
    		}
    		FileLock lock = fos.getChannel().tryLock();
    		
			if(lock != null){
				FileWriter fwriter = new FileWriter(fos.getFD());
				Date date = new Date();
				if(mode == 1){
					fwriter.write("["+(dateFormat.format(date))+"] "+line+"\n");
				}
				else{
					fwriter.write(line+"\n");
				}
				status = 1;

				//Release the lock and close the writer.
				lock.release();
				fwriter.close();
			}
			else{
				//The file is in use by another application, offer to try again.
				char option = 0;
				
				while(option != 'y' && option != 'Y' && option != 'n' && option != 'N'){
					option = console.prompt("The file is in use by another application, do you wish to try writing to it again? [Y/N]", "\\w").charAt(0);
				}
				
				switch(option){
				case 'Y':
				case 'y':
					write(filename, line, mode);
					break;
				case 'N':
				case 'n':
					status = 0;
					break;
				}
			}
			return status;
		} catch (IOException e) {
			System.out.println("WARNING! The lock for file could not be accessed. The file was not changed.");
			return 0;
		}
    }
	
	public String getPwd() {
		return this.pwd;
	}
}
