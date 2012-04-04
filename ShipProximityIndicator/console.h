/* 
 * File:   console.h
 * Author: sam
 *
 * Created on March 25, 2012, 11:42 PM
 */

#ifndef CONSOLE_H
#define	CONSOLE_H

#ifdef	__cplusplus
extern "C" {
#endif

    void clearScanBuffer();
    char promptChar(char [], char []);
    double promptDouble(char [], char []);
    char* promptStr(char *, char [], char []);

#ifdef	__cplusplus
}
#endif

#endif	/* CONSOLE_H */

