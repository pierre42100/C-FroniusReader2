#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

/**
 * Project configuration file
 *
 * @author Pierre HUBERT
 */

/**
 * Path to inverters configuration
 */
#define PATH_CONFIG_INVERTERS "config.json"

/**
 * The time of th break between values refresh (in seconds) (requests on server)
 */
#define BREAK_BETWEEN_REFRESH 1

/**
 * Time between two screen refresh (in seconds)
 */
#define TIME_BETWEEN_SCREEN_REFRESH 2

/**
 * Windows width
 */
#define WINDOW_WIDTH 310

/**
 * Path to the font file
 */
#define PATH_FONT_FILE "res/UbuntuMono-B.ttf"

/**
 * Size of the characters of the rendered text
 */
#define SIZE_CHARACTERS 30

/**
 * Name of the "total" inverter
 */
#define TOTAL_INVERTER_NAME "Total"

#endif // CONFIG_H_INCLUDED
