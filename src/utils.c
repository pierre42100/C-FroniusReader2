/**
 * Programm utilities
 *
 * @author Pierre HUBERT
 */
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "lib/jsmn/jsmn.h"
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

//Report error
void report_error(const char *message, int quit){

    //Display error
    fprintf(stderr, "An error occured: %s\n", message);

    //Display SDL error
    fprintf(stderr, "SDL error: %s\n", SDL_GetError());

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

/**
 * Display an error
 */
void error(const char *msg) {
    perror(msg);
    //exit(0);
}

//Perform a web request
char* web_request(int limit, const char *host, int port, const char *method, const char *path, const char *headers){

    /* first where are we going to send it? */
    int portno = port > 0 ? port : 80;

    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd, bytes, sent, received, total, message_size;
    char *message;
    char *response;

    //Allocate memory for response
    response = malloc(limit * sizeof(char));

    //Check memory was allocated
    if(response == NULL){
        error("Couldn't allocate memory to store response !");
        return NULL;
    }

    /* How big is the message? */
    message_size=0;
    if(!strcmp(method,"GET"))
    {
        message_size+=strlen("%s %s%s%s HTTP/1.0\r\n");        /* method         */
        message_size+=strlen(path);                         /* path           */
        message_size+=strlen("Host: \r\n");
        message_size+=strlen(host);
        message_size+=strlen(headers);                   /* headers        */
        message_size+=(strlen("\r\n"))*6;
        message_size+=strlen("\r\n");                          /* blank line     */
    }

    /* allocate space for the message */
    message=malloc(message_size);

    /* fill in the parameters */
    if(!strcmp(method,"GET"))
    {
        sprintf(message,"%s %s HTTP/1.0\r\n",
                method,               /* method         */
                path);                /* path           */

        //Add host
        strcat(message, "Host: ");
        strcat(message, host);
        strcat(message, "\r\n");

        strcat(message,"\r\n");                                /* blank line     */
    }


    /* What are we going to send? */
    //printf("Request:\n%s\n",message);

    /* create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket");
        return NULL;
    }

    /* lookup the ip address */
    server = gethostbyname(host);
    if (server == NULL){
        error("ERROR, no such host");
        return NULL;
    }

    /* fill in the structure */
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        error("ERROR connecting");
        return NULL;
    }

    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* receive the response */
    //memset(response,0,sizeof(response));
    total = limit-1;
    received = 0;
    do {
        bytes = read(sockfd,response+received,total-received);
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;

    } while (received < total);

    //Check if the response it too high
    if (received == total)
        fprintf(stderr, "Warning : response is too high to be completely stored! \n");

    /* close the socket */
    close(sockfd);

    //Free memory
    free(message);

    //Return response
    return response;
}

//Extract the production of an inverter from the response JSON file
int determine_inverter_produced_value(const char *response){

    //Create variables
    int i, j;
    char value[10];

    //Parse the file
    int foundLevel = 0;

    for(i = 0; response[i] != '\0'; i++){

        //Check to which level we can go
        if(
            (foundLevel == 0 && response[i] == '"') ||
            (foundLevel == 1 && response[i] == 'P') ||
            (foundLevel == 2 && response[i] == '_') ||
            (foundLevel == 3 && response[i] == 'P') ||
            (foundLevel == 4 && response[i] == 'V') ||
            (foundLevel == 5 && response[i] == '"') ||
            (foundLevel == 6 && response[i] == ' ') ||
            (foundLevel == 7 && response[i] == ':') ||
            (foundLevel == 8 && response[i] == ' ')
        )
            //Increase level
            foundLevel++;

        //Else we fall back to the first level
        else
            foundLevel = 0;

        //Check if we reached the last level
        if(foundLevel == 9){
            //Then we can quit loop
            i++;
            break;
        }

    }

    //Check if we managed to go to the last level
    if(foundLevel != 9){
        //An error occured
        return -1; //Couldn't get a value
    }

    //Extract the value from the array
    for(j = 0; response[i] != ','; j++){
        value[j] = response[i];
        value[j + 1] = '\0';

        //Increase i
        i++;

    }

    //Return result
    return atoi(value);
}
