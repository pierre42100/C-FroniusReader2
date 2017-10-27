#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

/**
 * User Interface functions
 *
 * @author Pierre HUBERT
 */

/**
 * Initializate the User Interface
 *
 * @return int -1 in case of failure
 */
int ui_init();

/**
 * Close the UI
 */
void ui_quit();


/**
 * Initializate background texture
 *
 * @param int w
 * @param int h -> width and height of the window
 * @return int -1 in case of failure
 */
int ui_init_background_texture(int w, int h);

/**
 * Reset screen : display only the background
 */
void ui_reset_screen();

#endif // UI_H_INCLUDED
