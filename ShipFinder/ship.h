/* 
 * File:   ship.h
 * Author: sam
 *
 * Created on March 28, 2012, 7:22 PM
 */

#ifndef SHIP_H
#define	SHIP_H

#include <iostream>

#include "log.h"

using namespace std;

class ship {
public:
    ship();
    ship(std::ifstream& fin);
    virtual ~ship();
    ship* getNext();

    string getMmsi();
    string getName();
    double getLat();
    double getLng();
    int getCourse();
    double getSpeed();

    void setNext(ship*);
    
private:
    string mmsi;
    string name;
    double lat;
    double lng;
    int course;
    double speed;
    ship *next;
};

#endif	/* SHIP_H */

