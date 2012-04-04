/* 
 * File:   main.c
 * Author: sam
 *
 * Created on March 25, 2012, 7:36 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "log.h"
#include "console.h"
#include "ship.h"

/*******************************************************************************
 * main.c
 * Sam Nicholls (msn)
 *
 * The proximity indicator application's main functionality.
 ******************************************************************************/

void launchSearch(log* l, ship** head);
void editWorkingDirectory(log* l);

/**
 * The application's run function. Displays the menu to the user and prompts
 * for a character response.
 *
 * @param argc
 * @param argv
 * @return Exit Code
 */
int main(int argc, char** argv) {

    log *l = malloc(sizeof(log));
    l = createLog();

    ship *head = malloc(sizeof(ship));
    head = NULL;

    writeLog(l, "[APP] The Proximity Indicator application was launched.\n");

    char mode = 0;
    while(mode != 'q' && mode != 'Q'){
        printf("            ,:',:`,:'\n");
        printf("         __||_||_||_||__\n");
        printf("    ____[\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"]____\n");
        printf("    \\ \" '''''''''''''''''''' \\\n");
        printf("  ~~^~^~^~^~^^~^~^~^~^~^~^~^~~^~^~^^~~^~^\n");
        printf("**************************************************\n");
        printf("St. George's Channel Navigation Information System\n");
        printf("            Proximity Indicator Module\n");
        printf("**************************************************\n");
        printf("S  Search for Ships by Proximity\n");
        printf("W  Change Working Directory\n");
        printf("Q  Quit");

        mode = promptChar("Please select an option", "%1c");

        switch(mode){
            case 'S':
            case 's':
                launchSearch(l, &head);
                break;
            case 'W':
            case 'w':
                editWorkingDirectory(l);
                break;
        }
        
    }
    writeLog(l, "[APP] The Proximity Indicator application was closed.\n");
    return (EXIT_SUCCESS);
}

/**
 * Prompts the user to enter a longitude and latitude to conduct the search.
 * Upon valid input, calls the "searchShips" function with the user response.
 * 
 * @param l A pointer to the application's log.
 * @param head Pointer-to-pointer of the head of the ship linked list.
 */
void launchSearch(log* l, ship** head){
    *head = buildShipList(l, *head);

    double lng = -360.0;
    double lat = -360.0;

    while(lat < -90.0 || lat > 90.0){
        lat = promptDouble("Latitude (-90.000 - 90.000)", "%lf");
    }
    
    while(lng < -180.0 || lng > 180.0){
        lng = promptDouble("Longitude (-180.000 - 180.000)", "%lf");
    }

    searchShips(l, *head, lat, lng);
    
    char option = 0;
    while(option != 's' && option != 'S' && option != 'x' && option != 'X'){
        option = promptChar("(S) Search Again, "
                "(X) Return to Main Menu", "%1c");
    }

    switch(option){
        case 'S':
        case 's':
            launchSearch(l, head);
            break;
        case 'X':
        case 'x':
            break;
    }
}

/**
 * Displays the current working directory and prompts the user for their choice
 * on whether they would like to change it. If this is the case, the function
 * accepts a new path for the PWD and updates the log file.
 * 
 * @param l A pointer to the application log file.
 */
void editWorkingDirectory(log* l){
    char option = 0;
    char newPwd[256] = "";
    char appPwdLine[256] = "";
    char appLogLine[256] = "";

    printf("\nThe current working directory is: \"");
    printf(l->pwd);
    printf("\"");

    while(option != 'y' && option != 'Y' && option != 'n' && option != 'N'){
        option = promptChar("Do you wish to change the present working "
                "directory? [Y/N]", "%1c");
    }

    switch(option){
        case 'Y':
        case 'y':
            promptStr(newPwd, "New Working Directory (Include Final "
                    "Directory Separator)", "%256s[^\n]");

            snprintf(appPwdLine, 256, "[PWD] The Proximity Indicator "
                    "application's PWD was changed to %s\n", newPwd);
            snprintf(appLogLine, 256, "[PWD] The Proximity Indicator "
                    "application log was changed to %slog.txt\n", newPwd);

            writeLog(l, appPwdLine);
            writeLog(l, appLogLine);

            //Change the PWD.
            strcpy(l->pwd, newPwd);

            printf("%s\n", "\nNOTE! For security, when the location of the log "
                    "file is changed, a notice is printed to the original log."
                    "\nIf this file did not exist, two warnings above will "
                    "display and can be safely disregarded.\n");
            displayAndLog(l, appPwdLine);
            displayAndLog(l, appLogLine);
            break;
        case 'N':
        case 'n':
            break;
    }
}
