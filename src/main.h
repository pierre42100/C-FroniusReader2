#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

/**
 * Fronius Reader V2 main header file
 *
 * @author Pierre HUBERT
 */

/**
 * Main function of the programm
 */
int main(int argc,char *argv[]);

/**
 * Main function of the "client" thread : it connects automatically
 * to all the inverters to retrieve the production values
 *
 * @param void *param Nothing
 */
void *client_thread(void *param);

#endif // MAIN_H_INCLUDED
