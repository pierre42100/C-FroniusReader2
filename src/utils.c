/**
 * Programm utilities
 *
 * @author Pierre HUBERT
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

//Report error
void report_error(const char *message, int quit){

    //Display error
    fprintf(stderr, "An error occured: %s\n", message);

    //Quit program if required
    quit == 1 ? exit(EXIT_FAILURE) : 0;

    return;

}

//File get contents
char* get_file_contents(const char *filename){

    //First, get file length
    FILE *file = NULL;
    file = fopen(filename, "r");

    //Check for errors
    if(file == NULL)
        report_error("Couldn't open configuration file !", 1);

    //Put cursor at the end of the file
    fseek(file, 0, SEEK_END);

    //Get the length of the file
    int file_length = ftell(file);

    //Close file
    fclose(file);

    //Allocate memory for the response
    char *response = NULL;
    response = malloc(sizeof(char) *(file_length+1));

    //Check allocation
    if(response == NULL)
        report_error("Couldn't allocate memory to store configuration!", 1);

    //Open the file
    int fs = open(filename, 0);
    if(fs == -1)
        report_error("Couldn't open configuration file for reading !", 1);

    //Read the file
    read(fs, response, file_length);

    //Close file
    close(fs);

    //Return result
    return response;
}
