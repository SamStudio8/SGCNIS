#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "console.h"
#include "log.h"

/*******************************************************************************
 * log.c
 * Sam Nicholls (msn)
 *
 * Functions pertaining to the creation and management of the application log.
 ******************************************************************************/

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

/**
 * Create a log struct to represent the application log.
 * Allocates memory and sets the initial working directory as "."
 * @return A pointer to this new log.
 */
log* createLog(){
    log *l = (log *) malloc(sizeof(log));
    strcpy(l->pwd, ".");
    return l;
}

/**
 * Simultaneously display information to the user and append to the log.
 * @param l Pointer to application log.
 * @param line The line to display to the user and append to the log.
 */
void displayAndLog(log* l, char line[256]){
    printf("%s", line);
    writeLog(l, line);
}

/**
 * Write to the log file.
 * This method will first try to obtain a lock on the log file; if it is free,
 * it will create a lock, write to the file and release it.
 * If it is not free, the user is warned and asked whether they wish to try
 * writing to the file again.
 *
 * @param l Pointer to the application log.
 * @param line The line to append to the log file.
 */
void writeLog(log* l, char line[256]){
    char path[256] = "";
    strcat(strcat(path, l->pwd), "log.txt");
    
    int logFileDescriptor = open(path, O_RDWR);
    if (logFileDescriptor == -1) {
        printf("WARNING! The log file could not be found.\nAll actions will go"
                " un-logged until this is rectified.\nYou may need to change "
                "the PWD with the 'W' command on the main menu.\n");
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
                    option = promptChar("The log file is in use by another "
                            "application, do you wish to try writing to it "
                            "again? [Y/N]", "%1c");
                }

                switch(option){
                    case 'Y':
                    case 'y':
                        writeLog(l, line);
                        break;
                    case 'N':
                    case 'n':
                        break;
                }
            } else {
                printf("\nWARNING! An unexpected error occurred writing to the"
                        " log file, your action was not appended.\n");
            }
        } else {
            //Generate timestamp.
            time_t t;
            struct tm * timestamp;
            char str_timestamp[80];

            time (&t);
            timestamp = localtime (&t);
            strftime(str_timestamp, 80, "[%d/%m/%y %X] ", timestamp);

            //Log file locked, write to file.
            FILE* logFile = fopen(path, "a+");
            fprintf(logFile, strcat(str_timestamp, line));

            //Release the file lock and close.
            fcntl(logFileDescriptor, F_SETLKW, file_lock(F_UNLCK, SEEK_SET));
            fclose(logFile);
        }
    }
}
