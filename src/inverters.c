/**
 * Inverters functions
 *
 * @author Pierre HUBERT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/jsmn/jsmn.h"
#include "inverters.h"
#include "config.h"
#include "utils.h"

//Last inverter
Inverter *last_inverter = NULL;

//Add an inverter to the list
void inverter_add(const char *url, const char *name){

    //Create inverter object
    Inverter *inverter = NULL;
    inverter = malloc(sizeof(Inverter));
    if(inverter == NULL)
        report_error("Couldn't allocate memory to store a new inverter!", 1);


    //First, we must retrieve URL informations
    if(extract_url_informations(url, inverter->hostname, inverter->path, &inverter->port) != 0)
        report_error("An error occured while retrieving URL informations !", 1);

    //Inform user
    printf("URL of the new inverter (%s): http://%s:%d%s\n", name, inverter->hostname, inverter->port, inverter->path);

    //Copy inverter name
    inverter->name = NULL;
    inverter->name = malloc(sizeof(char)*(strlen(name)+1));
    if(inverter->name == NULL)
        report_error("Couldn't allocate memory to store an inverter name!",1);
    strcpy(inverter->name, name);

    //Copy invert URL
    inverter->url = NULL;
    inverter->url = malloc(sizeof(char)*(strlen(url)+1));
    if(inverter->url == NULL)
        report_error("Couldn't allocate memory to store an inverter URL!",1);
    strcpy(inverter->url, url);

    //By default, the production is set to 0
    inverter->production = 0;

    //Save the pointer of the last inverter
    inverter->last_inverter = last_inverter;

    //Replace the pointer on the last inverter with the pointer of the current inverter
    last_inverter = inverter;

}

//Parse inverters configuration
int inverter_parse_config(const char *config){

    //Declare variables
    jsmn_parser parser;
    jsmntok_t *tokens = NULL;
    int tokens_number = 15;

    jsmn_init(&parser);

    int config_len = strlen(config);

    //Try to parse
    int parse_result = JSMN_ERROR_NOMEM;
    while(parse_result == JSMN_ERROR_NOMEM){

        //Allocate memory
        //For the first time
        if(tokens == NULL){
            tokens = malloc(sizeof(jsmntok_t)*tokens_number);

            if(tokens == NULL)
                report_error("Couldn't allocate memory to parse JSON code!", 1);
        }

        //In a case of reallocation
        else {

            //Inform user
            report_error("Not enough memory to parse JSON code, reallocating...", 0);

            //Add ten tokens
            tokens_number += 10;

            //Reallocation
            tokens = realloc(tokens, sizeof(jsmntok_t)*tokens_number);

        }

        parse_result = jsmn_parse(&parser, config, config_len, tokens, tokens_number);

    }

    //Check for errors
    if(parse_result == JSMN_ERROR_INVAL){
        report_error("Specified JSON code is invalid, therefore it can not be parsed !", 1);
    }
    if(parse_result == JSMN_ERROR_PART){
        report_error("Specified JSON code is to short, therefore it can not be parsed !", 1);
    }

    //Check the result can be processed
    if(parse_result < 1 || tokens[0].type != JSMN_ARRAY)
        report_error("Excepted JSON object at the root of the json file !", 1);


    //Parse results
    for(int i = 1; i < parse_result; i++){

        //Check if it is an object
        if(tokens[i].type != JSMN_OBJECT){
            report_error("JSON: Unexcepted object (required a an object, got something else) !", 0);
            continue;
        }

        //Prepare inverter processing
        char name[10] = "";
        char url[250] = "";

        //Process object
        int obj_size = tokens[i].size;

        //Increase counter (place it at the level of the first object entry)
        i++;

        for(int j = 1; j <= obj_size; j++){

            //Check what is the current key
            //For the name
            if(json_check_key("name", config, &tokens[i]) == 0 && tokens[i+1].type == JSMN_STRING){

                //Extract name
                strncpy(name, config + tokens[i+1].start, tokens[i+1].end - tokens[i+1].start);

            }
            //For the url
            else if(json_check_key("url", config, &tokens[i]) == 0 && tokens[i+1].type == JSMN_STRING){

                //Extract name
                strncpy(url, config + tokens[i+1].start, tokens[i+1].end - tokens[i+1].start);

            }
            else {
                report_error("A key was not recognized!", 0);
            }


            //Increase main counter
            i++;
            i++;

        }

        //Check if we got all the values
        if(strlen(url) > 2 && strlen(name) > 2)

            //Add the inverter to the list
            inverter_add(url, name);

        //Else display the error
        else

            report_error("Not enough informations to add an inverter to the list !", 0);

        //Decrease counter (was increased before)
        i--;


    }

    //Free memory
    free(tokens);

    //The operation is a success
    return 0;
}

//Refresh the informations of all the inverters
void inverter_refresh_all(){
    //Process all the list of currently installed inverters
    Inverter *current_inverter = last_inverter;

    while(current_inverter != NULL){
        //Refresh inverter
        inverter_refresh(current_inverter);

        //Go to the next inverter
        current_inverter = current_inverter->last_inverter;
    }
}

//Refresh production informations of a specified inverter
void inverter_refresh(Inverter *inverter){
    //Inform user
    printf("Refreshing production informations of %s.\n", inverter->name);

    //Perform web request
    char *server_response = web_request(1000, inverter->hostname, inverter->port, "GET", inverter->path, "");

    //Check if we got a response or not
    if(server_response == NULL){
        //We set the production to -1 (default response)
        inverter->production = -1;
    }
    else {

        //Determine production value
        inverter->production = determine_inverter_produced_value(server_response);

        //We must free memory
        free(server_response);

    }
}

//Display productions values as text in the terminal
void display_text_production_values(){
    int total_prod = 0;

    //Process the list of inverters
    Inverter *curr_inverter = last_inverter;

    while(curr_inverter != NULL){

        //Display the inverter
        printf("%s: \t %d\n", curr_inverter->name, curr_inverter->production);

        //Increment total production
        total_prod += curr_inverter->production;

        //Take the next inverter
        curr_inverter = curr_inverter->last_inverter;
    }

    //Print the total production
    printf("Total production:\t %d\n", total_prod);
}

//Get informations about the inverters name and length
void get_informations_inverters_name(int *name_length, int *number_inverters){

    //Inital length
    *name_length = strlen(TOTAL_INVERTER_NAME);
    *number_inverters = 1;

    //Process inverter list
    Inverter *current_inverter = last_inverter;

    while(current_inverter != NULL){

        *number_inverters = *number_inverters+1;

        int inverter_name_length = strlen(current_inverter->name);
        if(inverter_name_length > *name_length)
            *name_length = inverter_name_length;

        //Go to the next inverter
        current_inverter = current_inverter->last_inverter;
    }

}

//Get the name of all the inverters
char** get_inverters_name(int *num_names){

    //Get the number of inverters
    int number_inverters = 0;
    int name_length = 0;
    get_informations_inverters_name(&name_length, &number_inverters);

    //Include the "total" inverter in the list
    number_inverters++;

    //Initializate name array
    char* *names = NULL;
    names = malloc(number_inverters * sizeof(char *));

    //Process each inverter
    Inverter *curr_inverter = last_inverter;
    int count = 0;

    while(curr_inverter != NULL){

        //Allocate memory
        names[count] = NULL;
        names[count] = malloc(sizeof(char) * strlen(curr_inverter->name));
        if(names[count] == NULL)
            report_error("Error in memory allocation!", 1);

        //Copy inverter name in the new area
        strcpy(names[count], curr_inverter->name);

        //Go to the next inverter
        curr_inverter = curr_inverter->last_inverter;
        count++;

    }

    //Process the "total" inverter
    names[count] = malloc(strlen(TOTAL_INVERTER_NAME)*sizeof(char));
    strcpy(names[count], TOTAL_INVERTER_NAME);

    *num_names = number_inverters;
    return names;
}
