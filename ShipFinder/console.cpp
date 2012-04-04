/* 
 * File:   console.cpp
 * Author: sam
 * 
 * Created on March 27, 2012, 6:48 PM
 */

#include <iostream>
#include <sstream>

#include "console.h"

/*******************************************************************************
 * console.cpp
 * Sam Nicholls (msn)
 *
 * Definitions of member functions for the console object.
 ******************************************************************************/

using namespace std;

/**
 * Default console constructor.
 */
console::console() {
}

/**
 * Default console deconstructor.
 */
console::~console() {
}

/**
 * Prompt the user for a single character input.
 * @param promptFor The prompt to be displayed to the user.
 * @return User's character response.
 */
char console::promptChar(string promptFor){
    string input = "";

    while(input.length() != 1){
        cout << promptFor << ": ";
        getline(cin, input);
    }
    return input[0];
}

/**
 * Prompt the user for string input.
 * @param promptFor The prompt to be displayed to the user.
 * @return User's string response.
 */
string console::promptStr(string promptFor){
    string input = "";
    cout << promptFor << ": ";
    getline(cin, input);
    return input;
}