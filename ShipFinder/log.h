/* 
 * File:   log.h
 * Author: sam
 *
 * Created on March 27, 2012, 7:38 PM
 */

using namespace std;

#ifndef LOG_H
#define	LOG_H

struct flock* file_lock(short type, short whence);

class log {
public:
    log();
    virtual ~log();

    void writeLog(string line);
    string getPwd();
    void setPwd(string newPwd);
private:
    string pwd;

};

#endif	/* LOG_H */
