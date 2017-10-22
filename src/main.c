/**
 * Fronius Reader V2
 *
 * @author Pierre HUBERT
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "utils.h"

int main(int argc,char *argv[]){

    fprintf(stdout, "Fronius Reader V2\n(c) Pierre HUBERT 2017.\nLicense GNU-Linux v2.0 or older\n\n\n");

    //First, we must parse the configuration
    //Get the configuration content
    char *config_content = get_file_contents(PATH_CONFIG_INVERTERS);

}
