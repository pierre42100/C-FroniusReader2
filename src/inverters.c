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
#include "utils.h"

//Add an inverter to the list
void inverter_add(const char *url, const char *name){

    printf("Add inverter \"%s\" with url \"%s\" to the list...\n", name, url);

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
        if(strlen(url) > 2 & strlen(name) > 2)

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
