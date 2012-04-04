#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#include "log.h"
#include "console.h"
#include "ship.h"

/*******************************************************************************
 * ship.c
 * Sam Nicholls (msn)
 *
 * Functions for the creation of ship structs and the building of the list.
 ******************************************************************************/

/**
 * Create a ship struct.
 * @param fr The file from which to read the ship data.
 * @return A pointer to the new ship.
 */
ship* createShip(FILE* fr){
    ship *sh = (ship *) malloc(sizeof(ship));

    char line[256];
    
    fgets(line, 256, fr);
    sscanf(line, "%10s", sh->mmsi);

    fgets(line, 256, fr);
    sscanf(line, "%[^\n]", sh->name);

    fgets(line, 256, fr);
    sscanf(line, "%lf %lf %d %lf", &sh->lat, &sh->lng, &sh->course, &sh->speed);
    
    sh->next = NULL;
    return sh;
}

/**
 * Insert a ship to the next available space in the linked list.
 * @param node Pointer-to-pointer to the head of the linked list.
 * @param toInsert Pointer to the ship to insert.
 */
void insertShip(ship** node, ship* toInsert){
    if(*node == NULL){
        *node = toInsert;
    }
    else{
        insertShip(&(*node)->next, toInsert);
    }
}

/**
 * Build the linked list of ships from the ship data files in the directory.
 * The function makes a call to "readShipFile" for each txt file in the PWD that
 * does not contain the substring "log"; the readShipFile function handles file
 * locking and release procedures and works in the same way as the locking for
 * the application log file.
 *
 * @param l Pointer to the application log.
 * @param node Pointer to the head of the linked list
 * @return Pointer to the head of the linked list
 */
ship* buildShipList(log* l, ship* node){
    int freecount = 0;
    if(node != NULL){
        //If the head is not null, a list already exists, we should free it.
        while(node){
            ship* next_node = node->next;
            free(node);
            node = next_node;
            freecount++;
        }
        char message[256] = "";
        snprintf(message, 256, "[PRX] %d ship(s) freed from search list in "
                "Proximity Indicator application.\n", freecount);
        displayAndLog(l, message);
    }

    DIR *dir = NULL;
    struct dirent *entry = NULL;
    dir = opendir(l->pwd);

    FILE* current = NULL;
    int counter = 0;

    if(dir == NULL){
        //Cannot open directory.
        char message[256] = "";
        snprintf(message, 256, "[ERR] The Proximity Indicator Application was "
                "unable to open the working directory located at %s\n", l->pwd);
        displayAndLog(l, message);
    }
    else{
        while((entry = readdir(dir)) != NULL){ 
            if(((strstr(entry->d_name, ".txt")) != NULL)
                    && ((strstr(entry->d_name, "log")) == NULL)){

                char path[256] = "";
                strcat(strcat(path, l->pwd), entry->d_name);
                readShipFile(path, &current);
                
                if(current == NULL){
                    //Cannot open file.
                    char message[256] = "";
                    snprintf(message, 256, "[ERR] The Proximity Indicator "
                            "Application was unable to open the file "
                            "located at %s\n", path);
                    displayAndLog(l, message);
                }
                else{
                    insertShip(&node, createShip(current));
                    counter++;
                    fclose(current);
                }
            }

        }
        char message[256] = "";
        snprintf(message, 256, "[PRX] %d ship(s) added to search list in "
                "Proximity Indicator application.\n", counter);
        displayAndLog(l, message);
    }

    closedir(dir);
    return node;
}

/**
 * Search the linked list of ships for proximity to a given lng/lat co-ordinate.
 *
 * @param l Pointer to the application log.
 * @param node Pointer to the head of the linked list.
 * @param searchLat The latitude for which the user wishes to search at.
 * @param searchLng The longitude for which the user wishes to search at.
 */
void searchShips(log* l, ship* node, double searchLat, double searchLng){
    char message[256] = "";
    int matchCount = 0;

    snprintf(message, 256, "[PRX] Proximity Search Conducted at Location: "
            "LAT: %.3lf, LNG: %.3lf\n", searchLat, searchLng);
    displayAndLog(l, message);
    
    while(node){
        double diffLng = (searchLng - node->lng);
        double diffLat = (searchLat - node->lat);

        //Epsilon is a constant defined in the ship header
        //that corrects for the inaccuracy of FP storage.
        if(((diffLat <= PROXLAT+EPSILON)
                && (diffLat >= (-PROXLAT)-EPSILON))
                && ((diffLng <= PROXLNG+EPSILON)
                && (diffLng >= (-PROXLNG)-EPSILON))){

            matchCount++;
            message[256] = 0;
            snprintf(message, 256, "[PRX] Proximity Match Detected - "
                    "MMSI: %s (%s) LAT: %.3lf LNG: %.3lf "
                    "Course: %d Speed: %.1lf\n",
                    node->mmsi, node->name, node->lat, node->lng,
                    node->course, node->speed);
            displayAndLog(l, message);
        }
        node = node->next;
    }
    snprintf(message, 256, "[PRX] %d Proximity Match(es) "
            "Detected\n", matchCount);
    displayAndLog(l, message);
}

/**
 * Handles file locking and release for ship data files.
 * In very much the same way as access is controlled to the application's log
 * file, the function checks for a lock. If one is not found, it will make one,
 * read from the file and then release it. If another module is using the file,
 * it will prompt the user for a decision (try again or abort the read).
 * 
 * @param path The path of the ship data file to open in the shipData FILE ptr.
 * @param shipData Pointer-to-pointer to FILE which will be used to read the
 * ship data file in the calling function.
 */
void readShipFile(char path[256], FILE** shipData){
    int logFileDescriptor = open(path, O_RDWR);
    struct flock* lock = file_lock(F_WRLCK, SEEK_SET);
    if (fcntl(logFileDescriptor, F_SETLK, lock) == -1) {
        if (errno == EACCES || errno == EAGAIN) {
            //File locked.
            char option = 0;
            while(option != 'y' && option != 'Y'
                    && option != 'n' && option != 'N'){
                option = promptChar("A ship file is in use by another "
                        "application and could not be read.\nWithout reading "
                        "this file, the ship will not be included in search "
                        "methods.\nDo you wish to try reading from it "
                        "again? [Y/N]", "%1c");
            }
            switch(option){
                    case 'Y':
                    case 'y':
                        readShipFile(path, shipData);
                        break;
                    case 'N':
                    case 'n':
                        break;
            }
        }
    }
    else{
        *shipData = fopen(path, "r");
    }
}
