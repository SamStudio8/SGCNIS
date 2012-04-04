/* 
 * File:   ship.h
 * Author: sam
 *
 * Created on March 26, 2012, 10:13 PM
 */

#ifndef SHIP_H
#define	SHIP_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct ship{
        char mmsi[10];
        char name[80];
        double lat;
        double lng;
        int course;
        double speed;
        struct ship *next;
    }ship;

ship* buildShipList(log* l, ship* head);
ship* createShip(FILE* fr);
void searchShips(log* l, ship* node, double searchLat, double searchLng);
void insertShip(ship** node, ship* toInsert);
void readShipFile(char path[256], FILE** shipData);

#define EPSILON 0.0001
#define PROXLAT 0.1
#define PROXLNG 0.2

#ifdef	__cplusplus
}
#endif

#endif	/* SHIP_H */

