#ifndef INVERTERS_H_INCLUDED
#define INVERTERS_H_INCLUDED

/**
 * Inverters header file
 *
 * @author Pierre HUBERT
 */

typedef struct Inverter {

    //Name of the inverter
    char *name;

    //URL endpoint of the inverter
    char *url;
    char hostname[25];
    char path[150];
    int port;

    //Current production
    int production;

    //Last inverter of the list
    struct Inverter *last_inverter;

} Inverter;

/**
 * Add an inverter to the list
 *
 * @param const char *url The URL pointing on the inverter
 * @param const char *name Then name of the inverter
 */
void inverter_add(const char *url, const char *name);


/**
 * Parse configuration file
 *
 * @param const char *config The configuration to parse
 * @return int -1 in case of failure
 */
int inverter_parse_config(const char *config);

/**
 * Refresh the production informations of all the inverters
 */
void inverter_refresh_all();

/**
 * Refresh production informations of a specified inverter
 *
 * @param Inverter inverter The inverter to refresh
 */
void inverter_refresh(Inverter *inverter);

/**
 * Display the productions values within the terminal
 */
void display_text_production_values();

/**
 * Retrieve informations about the inverters name and number
 *
 * @param int *name_length This variable will be filled with the
 * biggest name length of the inverters
 * @param int *number_inverter This variable will be filled with
 * the number of inverters
 */
void get_informations_inverters_name(int *name_length, int *number_inverters);

/**
 * Returns the name of all the inverters
 *
 * @param int *num_names The number of names of the array
 * @return char[] An array with all the inverters name
 */
char** get_inverters_name(int *num_names);

#endif // INVERTERS_H_INCLUDED
