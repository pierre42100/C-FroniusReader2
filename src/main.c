/**
 * Fronius Reader V2
 *
 * @author Pierre HUBERT
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "config.h"
#include "utils.h"
#include "inverters.h"
#include "ui.h"
#include "main.h"

//Specify if the programm has to stop or not
int stop_program = 0;

int main(int argc,char *argv[]){

    fprintf(stdout, "Fronius Reader V2\n(c) Pierre HUBERT 2017.\nLicense GNU-Linux v2.0 or older\n\n\n");

    //First, we must parse the configuration
    //Get the configuration content
    char *config_content = get_file_contents(PATH_CONFIG_INVERTERS);

    //Parse the configuration
    inverter_parse_config(config_content);

    //Remove configuration string
    free(config_content);

    //Create client refresh thread
    pthread_t client_refresh_thread;
    if(pthread_create(&client_refresh_thread, NULL, *client_thread, NULL))
        report_error("Coudln't create client refresh thread !", 1);

    //Initializate User interface
    if(ui_init() < 0)
        report_error("Couldn't initializate User Interface!", 1);

    //Inform user
    printf("The application is ready.\n");

    sleep(25);

    //Quit ui
    ui_quit();
}

//Automated refresh client thread
void *client_thread(void *param){

    //Avoid errors
    (void) param;

    //This is a loop
    while(stop_program == 0){

        //Refresh the procution informations of all the inverters
        inverter_refresh_all();

        //Display in a textual way the total productions
        display_text_production_values();

        //Make a break
        sleep(BREAK_BETWEEN_REFRESH);
    }

    //Quit thread
    pthread_exit(EXIT_SUCCESS);

    //Return null result
    return NULL;
}
