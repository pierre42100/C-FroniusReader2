#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/**
 * Program utilities
 *
 * @author Pierre HUBERT
 */

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

#endif // UTILS_H_INCLUDED
