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
 *
 * @param int refresh_render Specify wether the renderer
 * should be applyed to the screen or not
 */
void ui_reset_screen(int refresh_render);

/**
 * Display production informations on the screen
 */
void ui_display_production();

#endif // UI_H_INCLUDED
