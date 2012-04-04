/* 
 * File:   application.cpp
 * Author: sam
 * 
 * Created on March 29, 2012, 11:50 AM
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#include "application.h"

/*******************************************************************************
 * application.cpp
 * Sam Nicholls (msn)
 *
 * Defintions of member functions for the application object.
 ******************************************************************************/

using namespace std;

/**
 * Default constructor.
 * Sets the reference member "headShip" to reference the ship pointer, "head".
 * Also sets the ship pointer, head, to be NULL.
 */
application::application() : headShip(head) {
    this->head = NULL;
}

/**
 * Default deconstructor.
 */
application::~application() {
}

/**
 * The application's run function.
 * Displays the menu and prompts the user for a character.
 */
void application::run(){
    l.writeLog("[APP] The Finder application was launched.\n");
    char mode = 0;
    while(mode != 'q' && mode != 'Q'){
        cout << "            ,:',:`,:'\n";
        cout << "         __||_||_||_||__\n";
        cout << "    ____[\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"]____\n";
        cout << "    \\ \" '''''''''''''''''''' \\\n";
        cout << "  ~~^~^~^~^~^^~^~^~^~^~^~^~^~~^~^~^^~~^~^\n";
        cout << "**************************************************\n";
        cout << "St. George's Channel Navigation Information System\n";
        cout << "                  Finder Module\n";
        cout << "**************************************************\n";
        cout << "F  Find Ship by MMSI\n";
        cout << "S  Search for Ships by Name\n";
        cout << "W  Change Working Directory\n";
        cout << "Q  Quit\n";

        mode = c.promptChar("Please select an option");

        string input = "";
        switch(mode){
            case 'F':
            case 'f':
                while(input.length() != 9){
                    input = c.promptStr("MMSI");
                }
                this->searchShipMmsi(head, input);
                break;
            case 'S':
            case 's':
                this->searchShipName(head, 
                        c.promptStr("Name (Case Sensitive)"));
                break;
            case 'W':
            case 'w':
                this->editWorkingDirectory();
                break;
        }
    }
    l.writeLog("[APP] The Finder application was closed.\n");
}

/**
 * The application's search by MMSI function.
 * The will call to build the linked list of ships with the ship data files
 * stored in the application's present working directory before examining each
 * ship in the list for an MMSI that matches the given user input.
 * Matches are logged and displayed and the list is then freed.
 *
 * @param node The head of the linked list of ships.
 * @param mmsi The MMSI for which to search for.
 */
void application::searchShipMmsi(ship* node, string mmsi){
    node = buildShipList(headShip);
    int matchCount = 0;

    stringstream message;
    message << "[FND] Finder Search Conducted with MMSI: " << mmsi << "\n";
    displayAndLog(message.str());

    //Clear the stringstream!
    message.str("");
    message.clear();

    while(node){
        if(mmsi.compare(node->getMmsi()) == 0){
            message << "[FND] Finder Match Detected - MMSI: "
                    << node->getMmsi() << " (" << node->getName() << ") "
                    << "LAT: " << node->getLat() << " LNG: " << node->getLng()
                    << " Course: " << node->getCourse() << " Speed: "
                    << node->getSpeed() << "\n";

            displayAndLog(message.str());
            message.str("");
            message.clear();
            matchCount++;
        }
        node = node->getNext();
    }
    message << "[FND] " << matchCount << " Finder Match(es) Detected\n";
    displayAndLog(message.str());
    message.str("");
    message.clear();
    freeShipList(head);

    char option = 0;
    while(option != 'f' && option != 'F' && option != 'x' && option != 'X'){
        option = c.promptChar("\n(F) Find another MMSI, "
                "(X) Return to Main Menu");
    }

    string input = "";
    switch(option){
        case 'F':
        case 'f':
            while(input.length() != 9){
                    input = c.promptStr("MMSI");
            }
            this->searchShipMmsi(head, input);
            break;
        case 'X':
        case 'x':
            break;
    }
}

/**
 * The application's search by name function.
 * The will call to build the linked list of ships with the ship data files
 * stored in the application's present working directory before examining each
 * ship in the list for a name that contains the given user input.
 * Matches are logged and displayed and the list is then freed.
 *
 * @param node The head of the linked list of ships.
 * @param name The name for which to search for.
 */
void application::searchShipName(ship* node, string name){
    node = buildShipList(headShip);
    int matchCount = 0;

    stringstream message;
    message << "[FND] Finder Search Conducted with Name: " << name << "\n";
    displayAndLog(message.str());

    //Clear the stringstream!
    message.str("");
    message.clear();

    while(node){
        if(node->getName().find(name) != string::npos){
            message << "[FND] Finder Match Detected - Name: "
                    << node->getName() << " (" << node->getMmsi() << ") "
                    << "LAT: " << node->getLat() << " LNG: " << node->getLng()
                    << " Course: " << node->getCourse() << " Speed: "
                    << node->getSpeed() << "\n";

            displayAndLog(message.str());
            message.str("");
            message.clear();
            matchCount++;
        }
        node = node->getNext();
    }
    message << "[FND] " << matchCount << " Finder Match(es) Detected\n";
    displayAndLog(message.str());
    message.str("");
    message.clear();
    freeShipList(head);
    
    char option = 0;
    while(option != 's' && option != 'S' && option != 'x' && option != 'X'){
        option = c.promptChar("\n(S) Search another Name, "
                "(X) Return to Main Menu");
    }

    switch(option){
        case 'S':
        case 's':
            this->searchShipName(head, c.promptStr("Name (Case Sensitive)"));
            break;
        case 'X':
        case 'x':
            break;
    }
}

/**
 * Displays the current working directory, and prompts the user for their choice
 * on whether they would like to change it. If this is the case, the function
 * accepts a new path for the PWD and updates the log file.
 */
void application::editWorkingDirectory(){
    char option = 0;
    string newPwd = "";
    string appPwdLine = "";
    string appLogLine = "";

    cout << "The current working directory is \"" << l.getPwd() << "\"\n";

    while(option != 'y' && option != 'Y' && option != 'n' && option != 'N'){
        option =  c.promptChar("Do you wish to change the present "
                "working directory? [Y/N]");
    }

    switch(option){
        case 'Y':
        case 'y':
            newPwd = c.promptStr("New Working Directory (Include "
                    "Final Directory Separator)");

            appPwdLine = "[PWD] The Finder application's PWD was "
                    "changed to" + newPwd + "\n";
            appLogLine = "[PWD] The Finder application log was "
                    "changed to" + newPwd + "log.txt\n";

            l.writeLog(appPwdLine);
            l.writeLog(appLogLine);

            //Change the PWD.
            l.setPwd(newPwd);

            cout << "\nNOTE! For security, when the location of the log "
                    "file is changed, a notice is printed to the original log."
                    "\nIf this file did not exist, two warnings above will "
                    "display and can be safely disregarded.\n";
            displayAndLog(appPwdLine);
            displayAndLog(appLogLine);
            
            break;
        case 'N':
        case 'n':
            break;
    }
}

/**
 * Build the linked list of ships from the ship data files in the directory.
 * The function makes a call to "readShipFile" for each txt file in the PWD that
 * does not contain the substring "log"; the readShipFile function handles file
 * locking and release procedures and works in the same way as the locking for
 * the application log file.
 *
 * @param node A references to the head of the linked list
 * @return A pointer to the head of the linked list
 */
ship* application::buildShipList(ship*& node){

    DIR *dir = NULL;
    struct dirent *entry = NULL;
    dir = opendir(l.getPwd().c_str());

    int counter = 0;

    if(dir == NULL){
        //Cannot open directory.
        string message = "[ERR] The Finder Application was unable to open "
                "the working directory located at " + l.getPwd() + "\n";
        displayAndLog(message);
    }
    else{
        while((entry = readdir(dir)) != NULL){
            string fileName (entry->d_name);
            if((fileName.find(".txt") != string::npos)
                    && (fileName.compare("log.txt") != 0)){

                ifstream current;
                string path = l.getPwd() + entry->d_name;
                readShipFile(path, current);

                if(current == NULL){
                    //Cannot open file.
                    string message = "[ERR] The Finder Application was unable "
                            "open the to file located at "+ path + "\n";
                    displayAndLog(message);
                }
                else{
                    insertShip(node, new ship(current));
                    counter++;
                    current.close();
                }
            }
        }
        stringstream message;
        message << "[FND] " << counter << " ship(s) added to search list "
                "in Finder application.\n";
        displayAndLog(message.str());
    }
    closedir(dir);
    return node;
}

/**
 * Simultaneously display information to the user and append to the log.
 * @param line The string to display and log.
 */
void application::displayAndLog(string line){
    cout << line;
    l.writeLog(line);
}

/**
 * Insert a ship to the linked list in the next available "->next" position.
 * @param node A reference to the head of the linked list.
 * @param toInsert The new ship to insert.
 */
void application::insertShip(ship*& node, ship* toInsert){
    if(node == NULL){
        node = toInsert;
    }
    else{
        ship* temp = node;
        while(temp->getNext() != NULL){
            temp = temp->getNext();
        }
        temp->setNext(toInsert);
    }
}

/**
 * Free the memory of each member of the linked list.
 * @param node A reference to the head of the linked list.
 */
void application::freeShipList(ship*& node){
    int freecount = 0;
    if(node != NULL){
        //If the head is not null, a list already exists, we should free it.
        while(node){
            ship* next_node = node->getNext();
            delete(node);
            node = next_node;
            freecount++;
        }
        stringstream message;
        message << "[FND] " << freecount << " ship(s) freed from search list "
                "in Finder application.\n";
        displayAndLog(message.str());
    }
}

/**
 * Handles file locking and release for ship data files.
 * In very much the same way as access is controlled to the application's log
 * file, the function checks for a lock. If one is not found, it will make one,
 * read from the file and then release it. If another module is using the file,
 * it will prompt the user for a decision (try again or abort the read).
 *
 * @param path The path of the ship data file to open in the shipData ifstream.
 * @param shipData An reference to an ifstream that will be used to read from
 * the file in the calling function.
 */
void application::readShipFile(string path, ifstream& shipData){
    int logFileDescriptor = open(path.c_str(), O_RDWR);
    struct flock* lock = file_lock(F_WRLCK, SEEK_SET);
    if (fcntl(logFileDescriptor, F_SETLK, lock) == -1) {
        if (errno == EACCES || errno == EAGAIN) {
            //File locked.
            char option = 0;
            while(option != 'y' && option != 'Y'
                    && option != 'n' && option != 'N'){
                option = c.promptChar("A ship file is in use by another "
                        "application and could not be read.\nWithout reading "
                        "this file, the ship will not be included in search "
                        "methods.\nDo you wish to try reading from it "
                        "again? [Y/N]");
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
        else{
            string message;
            message = "[ERR] The Finder Application experienced an unexpected"
                    " error attempting to open the ship data file located "
                    "at " + path + "\n";
            displayAndLog(message);
        }
    }
    else{
        shipData.open(path.c_str(), ios::in);
    }
}
