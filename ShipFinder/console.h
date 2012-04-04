/* 
 * File:   console.h
 * Author: sam
 *
 * Created on March 27, 2012, 6:48 PM
 */

#include <string>

using namespace std;

#ifndef CONSOLE_H
#define	CONSOLE_H

class console {
public:

    console();
    virtual ~console();

    char promptChar(string promptFor);
    string promptStr(string promptFor);
    
    
private:

};

#endif	/* CONSOLE_H */

