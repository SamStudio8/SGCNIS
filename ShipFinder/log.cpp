/* 
 * File:   log.cpp
 * Author: sam
 * 
 * Created on March 27, 2012, 7:38 PM
 */

#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "log.h"
#include "console.h"
#include "reader.h"

/*******************************************************************************
 * log.cpp
 * Sam Nicholls (msn)
 *
 * Defintions of member functions for the log object.
 ******************************************************************************/

using namespace std;

/**
 * Default log constructor.
 * Initially sets the application's present working directory to "."
 */
log::log() {
    this->pwd = ".";
}

/**
 * Default log deconstructor.
 */
log::~log() {
}

/**
 * Write to the log file.
 * This method will first try to obtain a lock on the log file; if it is free,
 * it will create a lock, write to the file and release it.
 * If it is not free, the user is warned and asked whether they wish to try
 * writing to the file again.
 * @param line The string to write to the log file.
 */
void log::writeLog(string line){

    console c;
    string path = this->pwd + "log.txt";
    
    int logFileDescriptor = open(path.c_str(), O_RDWR);
    if (logFileDescriptor == -1) {
        cout << "WARNING! The log file could not be found.\nAll actions will"
                " go un-logged until this is rectified.\nYou may need to change"
                " the PWD with the 'W' command on the main menu.\n";
    }
    else{
        //Check the lock.
        struct flock* lock = file_lock(F_WRLCK, SEEK_SET);
        if (fcntl(logFileDescriptor, F_SETLK, lock) == -1) {
            if (errno == EACCES || errno == EAGAIN) {
                //File locked by another application.
                char option = 0;
                while(option != 'y' && option != 'Y'
                        && option != 'n' && option != 'N'){
                    option = c.promptChar("The log file is in use by another "
                            "application, do you wish to try writing to it "
                            "again? [Y/N]");
                }

                switch(option){
                    case 'Y':
                    case 'y':
                        writeLog(line);
                        break;
                    case 'N':
                    case 'n':
                        break;
                }
            } else {
                cout << "\nWARNING! An unexpected error occurred writing to "
                        "the log file, your action was not appended.\n";
            }
        } else {
            //Generate timestamp.
            time_t t;
            struct tm * timestamp;
            char str_timestamp[80];

            time(&t);
            timestamp = localtime(&t);
            strftime(str_timestamp, 80, "[%d/%m/%y %X] ", timestamp);

            //Log file locked, write to file.
            ofstream logFile (path.c_str(), ios::out | ios::app);
            logFile << str_timestamp << line;

            //Release the file lock and close.
            fcntl(logFileDescriptor, F_SETLKW, file_lock(F_UNLCK, SEEK_SET));
            logFile.close();
        }
    }
}

/**
 * Get the present working directory.
 * @return The present working directory of the log.
 */
string log::getPwd(){
    return this->pwd;
}

/**
 * Set the present working directory.
 * @param newPwd The new path to the present working directory.
 */
void log::setPwd(string newPwd){
    this->pwd = newPwd;
}

/**
 * Functionality provided via Blackboard example used to construct a file lock.
 * @param type Lock Type (eg. F_RDLCK)
 * @param whence Seek offset relation argument (eg. SEEK_SET)
 * @return A pointer to the created file lock.
 */
struct flock* file_lock(short type, short whence){
    static struct flock ret;
    ret.l_type = type;
    ret.l_start = 0;
    ret.l_whence = whence;
    ret.l_len = 0;
    ret.l_pid = getpid();
    return &ret;
}
