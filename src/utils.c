/**
 * Programm utilities
 *
 * @author Pierre HUBERT
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "lib/jsmn/jsmn.h"

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

//Check two JSON keys
int json_check_key(const char *name, const char *json, jsmntok_t *token){

    return (
        token->type == JSMN_STRING &&
        (int) strlen(name) == token->end - token->start &&
        strncmp(name, json + token->start, token->end - token->start) == 0

    ) ? 0 : -1;

}

//Extract URL informations
int extract_url_informations(const char *src_url, char *hostname, char *path, int *port){

    //Make a copy of the URL string
    char *url;
    url = malloc(strlen(src_url) * sizeof(char));
    if(url == NULL)
        report_error("Couldn't allocate memory to copy URL!", 1);
    strcpy(url, src_url);

    //First, determine if there is "http://" at the begining of the url
    //in order to remove it
    if(strncmp(url, "http://", 7) == 0){
        //Then remove it
        strcpy(url, url + 7);
    }

    //Parse the string
    int current_process = 1;
    int hostname_pos = 0;
    char str_port[5] = {};
    int port_pos = 0;
    int path_pos = 0;
    for(int i = 0; i < strlen(url); i++){

        //Check if we reached the end of the string
        if(url[i] == '\0')
            break;

        //Check if we are accessing the port of the connexion
        if(url[i] == ':'){
            current_process = 2;
            continue;
        }

        //Check if we are accession the path of the url
        if(url[i] == '/')
            current_process = 3;

        //For the hostname
        if(current_process == 1){
            hostname[hostname_pos] = url[i];
            hostname_pos++;
        }

        //For the port
        if(current_process == 2){
            str_port[port_pos] = url[i];
            port_pos++;
        }

        //For the path
        if(current_process == 3){
            path[path_pos] = url[i];
            path_pos++;
        }

    }

    //Add the end of string characters
    hostname[hostname_pos] = '\0';
    str_port[port_pos] = '\0';
    path[path_pos] = '\0';

    //Convert the port into an integer
    if(strlen(str_port)>1){

        int computed_port = 0;
        for(int k = 0; k < strlen(str_port); k++){
            computed_port += (int) (str_port[k] - '0')*pow(10, strlen(str_port) - k - 1);
        }
        *port = computed_port;
    }
    else {
        *port = 80;
    }

    //Free memory
    free(url);

    //Success
    return 0;
}
