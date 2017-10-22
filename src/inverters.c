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

    //Free memory
    free(tokens);

    //The operation is a success
    return 0;
}
