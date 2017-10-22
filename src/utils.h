#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/**
 * Program utilities
 *
 * @author Pierre HUBERT
 */

#include "lib/jsmn/jsmn.h"

/**
 * Report errors
 *
 * @param const char *message Error message
 * @param int quit Specify wether the program should exit once the error was returned
 */
void report_error(const char *message, int quit);

/**
 * Get a file content
 *
 * @param const char *filename The name of the file to read
 * @return *char File content
 */
char* get_file_contents(const char *filename);

/**
 * Check if a json key match with a string
 *
 * @param const char *name The name to test
 * @param const char *json The JSON code to test
 * @param jsmntok_t *token The token to check
 * @return int 0 if match -1 else
 */
int json_check_key(const char *name, const char *json, jsmntok_t *token);

/**
 * Extract URL informations
 *
 * @param const char *src_url The URL to process
 * @param char **hostname Hostname
 * @param char **path Path
 * @param int **port Port
 * @return int 0 on success / -1 on failure
 */
int extract_url_informations(const char *src_url, char *hostname, char *path, int *port);

#endif // UTILS_H_INCLUDED
