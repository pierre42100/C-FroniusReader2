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

    //First, we must parse the configuration
    //Get the configuration content
    char *response = get_file_contents(PATH_CONFIG_INVERTERS);

    printf("%s", response);

}
