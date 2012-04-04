/* 
 * File:   main.cpp
 * Author: sam
 *
 * Created on March 27, 2012, 6:07 PM
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <dirent.h>

#include "application.h"
#include "console.h"
#include "log.h"
#include "ship.h"

/*******************************************************************************
 * main.cpp
 * Sam Nicholls (msn)
 *
 * Launches an instance of the Finder application and calls its run function.
 ******************************************************************************/

using namespace std;

/**
 * Creates an instance of an application object and calls its run function.
 * Upon termination the memory allocated to the application object is freed.
 * 
 * @param argc
 * @param argv
 * @return Exit Code
 */
int main(int argc, char** argv) {
    application *a = new application();
    a->run();
    free(a);
    return 0;
}

