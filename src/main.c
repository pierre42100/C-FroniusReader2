/**
 * Fronius Reader V2
 *
 * @author Pierre HUBERT
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "utils.h"
#include "inverters.h"

int main(int argc,char *argv[]){

    fprintf(stdout, "Fronius Reader V2\n(c) Pierre HUBERT 2017.\nLicense GNU-Linux v2.0 or older\n\n\n");

    //First, we must parse the configuration
    //Get the configuration content
    char *config_content = get_file_contents(PATH_CONFIG_INVERTERS);

    //Parse the configuration
    inverter_parse_config(config_content);

    //Remove configuration string
    free(config_content);

    //Inform user
    printf("The application is ready.\n");

    //Refresh the procution informations of all the inverters
    inverter_refresh_all();

    //Display in a textual way the total productions
    display_text_production_values();

}
