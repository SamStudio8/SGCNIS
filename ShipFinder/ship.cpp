/* 
 * File:   ship.cpp
 * Author: sam
 * 
 * Created on March 28, 2012, 7:22 PM
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "ship.h"

/*******************************************************************************
 * ship.cpp
 * Sam Nicholls (msn)
 *
 * Definitions of member functions for the ship object.
 ******************************************************************************/

using namespace std;

/**
 * Default constructor for ship.
 * Sets the pointer to the next ship as NULL.
 */
ship::ship(){
    this->next = NULL;
}

/**
 * Default deconstructor for ship.
 */
ship::~ship() {
}

/**
 * Constructor for ship with input stream parameter used to read ship file data.
 * @param fin The std::ifstream from which to read the ship data.
 */
ship::ship(std::ifstream& fin) {
    string line = "";

    getline(fin, this->mmsi);
    getline(fin, this->name);

    getline(fin, line);
    sscanf(line.c_str(), "%lf %lf %d %lf",
            &this->lat, &this->lng, &this->course, &this->speed);

    this->next = NULL;
}

/**
 * Get the MMSI of this ship.
 * @return The MMSI of this ship.
 */
string ship::getMmsi(){
    return this->mmsi;
}

/**
 * Get the name of this ship.
 * @return The name of this ship.
 */
string ship::getName(){
    return this->name;
}

/**
 * Get the latitude of this ship.
 * @return The latitude of this ship.
 */
double ship::getLat(){
    return this->lat;
}

/**
 * Get the longitude of this ship.
 * @return The longitude of this ship.
 */
double ship::getLng(){
    return this->lng;
}

/**
 * Get the course of this ship.
 * @return The course of this ship.
 */
int ship::getCourse(){
    return this->course;
}

/**
 * Get the speed of this ship.
 * @return The speed of this ship.
 */
double ship::getSpeed(){
    return this->speed;
}

/**
 * Get the pointer to the next ship in the list.
 * @return A pointer to the ship this ship defines as its "next" ship.
 */
ship* ship::getNext(){
    return this->next;
}

/**
 * Set the pointer to the next ship in the list.
 * @param next A pointer to the ship that is to be set as this ship's "next".
 */
void ship::setNext(ship* next){
    this->next = next;
}
