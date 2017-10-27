/**
 * User interface functions
 *
 * @author Pierre HUBERT
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "config.h"
#include "utils.h"
#include "inverters.h"
#include "ui.h"

static SDL_Renderer *renderer = NULL;
static SDL_Window *window = NULL;
static TTF_Font *font = NULL;
static SDL_Texture *background_texture = NULL;

//Initializate User Interface
int ui_init(){

    //Initializate SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        return -1;

    //Initializate SDL TTF
    if(TTF_Init() != 0)
        return -1;

    //Load the font library
    font = TTF_OpenFont(PATH_FONT_FILE, SIZE_CHARACTERS);

    //Check for errors
    if(font == NULL)
        return -1;

    //Retrieve informations about the inverters to determine window dimensions
    int inverter_name_length = 0;
    int number_inverters = 0;
    get_informations_inverters_name(&inverter_name_length, &number_inverters);
    int window_height = number_inverters*TTF_FontHeight(font);
    int window_width = SIZE_CHARACTERS*(inverter_name_length+7);

    //Create a window & its renderer
    if(SDL_CreateWindowAndRenderer(window_width, window_height, SDL_WINDOWPOS_CENTERED, &window, &renderer) != 0)
        return -1;

    //Initializate background texture
    if(ui_init_background_texture(window_width, window_height) != 0)
        return -1;

    //Reset background screen
    ui_reset_screen();

    return 0;
}

//Close UI
void ui_quit(){

    if(renderer != NULL)
        SDL_DestroyRenderer(renderer);

    if(window != NULL)
        SDL_DestroyWindow(window);

    if(font != NULL)
        TTF_CloseFont(font);

    SDL_Quit();

}

//Initializate background texture
int ui_init_background_texture(int w, int h){

    background_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

    //Check for errors
    if(background_texture == NULL)
        return -1;

    //Change renderer target
    SDL_SetRenderTarget(renderer, background_texture);

    //Fill background in black
    SDL_Color black = {0, 0, 0, 0};
    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    SDL_RenderClear(renderer);

    //Get inverters name
    int num_names = 0;
    char* *inverters_name = get_inverters_name(&num_names);

    //Process each inverter name
    for(int i = 0; i < num_names-1; i++){
        if(inverters_name[i] != NULL)
            printf("Inverter name: %s\n", inverters_name[i]);
    }

    //Free memory
    for(int i = 0; i < num_names; i++){
        if(inverters_name[i] != NULL)
            free(inverters_name[i]);
    }


    //Reset renderer target
    SDL_SetRenderTarget(renderer, NULL);

    //This is a success
    return 0;
}

//Reset background
void ui_reset_screen(){
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, background_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
