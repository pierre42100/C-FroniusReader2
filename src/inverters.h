#ifndef INVERTERS_H_INCLUDED
#define INVERTERS_H_INCLUDED

/**
 * Inverters header file
 *
 * @author Pierre HUBERT
 */

typedef struct Inverter {

    //Name of the inverter
    *char name;

    //URL endpoint of the inverter
    *char url;

    //Current production
    int production;

} Inverter;


/**
 * Parse configuration file
 *
 * @param const char *config The configuration to parse
 * @return int -1 in case of failure
 */
int inverter_parse_config(const char *config);

#endif // INVERTERS_H_INCLUDED
