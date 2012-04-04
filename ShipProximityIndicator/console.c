#include <stdio.h>

/*******************************************************************************
 * console.c
 * Sam Nicholls (msn)
 *
 * Functions used to prompt the user for a particular input.
 ******************************************************************************/

/**
 * Clears any remaining characters from the scan buffer.
 */
void clearScanBuffer(){
    scanf("%*[^\n]"); //Clear up to the next newline
    getchar(); //Remove the newline
}

/**
 * Prompt the user for a single character input.
 * @param promptFor The prompt to be displayed to the user.
 * @param parseFor The scanf format the input should match.
 * @return User's character response.
 */
char promptChar(char promptFor[], char parseFor[]){
    char output = 0;
    int status = 0;

    while(status != 1){
        printf("\n%s: ", promptFor);
        status = scanf(parseFor, &output);

        if(status != 1){
            clearScanBuffer();
        }
    }
    clearScanBuffer();
    return output;
}

/**
 * Prompt the user for input of a double.
 * @param promptFor The prompt to be displayed to the user.
 * @param parseFor The scanf format the input should match.
 * @return User's double response.
 */
double promptDouble(char promptFor[], char parseFor[]){
    double output = 0;
    int status = 0;

    while(status != 1){
        printf("%s: ", promptFor);
        status = scanf(parseFor, &output);

        if(status != 1){
            clearScanBuffer();
        }
    }
    clearScanBuffer();
    return output;
}

/**
 * Prompt the user for input of a character array.
 * @param promptFor The prompt to be displayed to the user.
 * @param parseFor The scanf format the input should match.
 * @return User's char[] response.
 */
void promptStr(char *str, char promptFor[], char parseFor[]){
    int status = 0;

    while(status != 1){
        printf("\n%s: ", promptFor);
        status = scanf(parseFor, str);

        if(status != 1){
            clearScanBuffer();
        }
    }
    clearScanBuffer();
}
