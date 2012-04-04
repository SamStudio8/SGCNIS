/* 
 * File:   application.h
 * Author: sam
 *
 * Created on March 29, 2012, 11:50 AM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

#include "log.h"
#include "console.h"
#include "ship.h"

class application {
public:
    application();
    virtual ~application();

    void run();
    void displayAndLog(string line);
    void readShipFile(string path, ifstream& shipData);
    
    void editWorkingDirectory();
    void searchShipMmsi(ship*, string);
    void searchShipName(ship*, string);
    
    ship* buildShipList(ship*& node);
    void insertShip(ship*& node, ship* toInsert);
    void freeShipList(ship*& node);
    
private:
    log l;
    console c;

    ship* head;
    ship* &headShip;
};

#endif	/* APPLICATION_H */
