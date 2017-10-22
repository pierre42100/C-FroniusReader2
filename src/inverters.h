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

#endif // INVERTERS_H_INCLUDED
