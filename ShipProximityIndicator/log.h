/* 
 * File:   log.h
 * Author: sam
 *
 * Created on March 25, 2012, 8:49 PM
 */

#ifndef LOG_H
#define	LOG_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct log{
    char pwd[256];
}log;
    
struct flock* file_lock(short type, short whence);
log* createLog();
void displayAndLog(log* l, char line[256]);
void writeLog(log* l, char line[256]);
void setPwd(log* l);

#ifdef	__cplusplus
}
#endif

#endif	/* LOG_H */

