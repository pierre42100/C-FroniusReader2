/**
 * User interface functions
 *
 * @author Pierre HUBERT
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "config.h"
#include "utils.h"
#include "inverters.h"
#include "ui.h"

static SDL_Renderer *renderer = NULL;
static SDL_Window *window = NULL;
static TTF_Font *font = NULL;
static SDL_Texture *background_texture = NULL;
static int number_inverters = 0;

static SDL_Color white = {255, 255, 255, 1};

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
    //int number_inverters = 0;
    get_informations_inverters_name(&inverter_name_length, &number_inverters);
    int window_height = number_inverters*TTF_FontHeight(font);
    int window_width = WINDOW_WIDTH;

    //Create a window & its renderer
    if(SDL_CreateWindowAndRenderer(window_width, window_height, SDL_WINDOWPOS_CENTERED, &window, &renderer) != 0)
        return -1;

    //Initializate background texture
    if(ui_init_background_texture(window_width, window_height) != 0)
        return -1;

    //Reset background screen
    ui_reset_screen(1);

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
    for(int i = 0; i < num_names; i++){
        if(inverters_name[i] != NULL){

            //Write inverter name on texture
            //printf("Inverter name: %s\n", inverters_name[i]);
            SDL_Surface *rendered_surface = TTF_RenderText_Blended(font, inverters_name[i], white);

            if(rendered_surface == NULL)
                report_error("Error while rendering text!", 1);

            //Convert surface into texture
            SDL_Texture *rendered_texture = SDL_CreateTextureFromSurface(renderer, rendered_surface);

            //Copy the newly created texture on the background texture
            SDL_Rect dest = {0, i*TTF_FontHeight(font), rendered_surface->w, rendered_surface->h};
            SDL_RenderCopy(renderer, rendered_texture, NULL, &dest);

            //Free memory
            SDL_DestroyTexture(rendered_texture);
            SDL_FreeSurface(rendered_surface);

        }
    }

    //Free memory
    for(int i = 0; i < num_names; i++){
        if(inverters_name[i] != NULL)
            free(inverters_name[i]);
    }
    free(inverters_name);

    //Reset renderer target
    SDL_SetRenderTarget(renderer, NULL);

    //This is a success
    return 0;
}

//Reset background
void ui_reset_screen(int refresh_render){
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, background_texture, NULL, NULL);

    if(refresh_render == 1)
        SDL_RenderPresent(renderer);
}

//Display production informations on the screen
void ui_display_production(){

    //Reset background
    ui_reset_screen(0);

    //Make the renderer as the target
    SDL_SetRenderTarget(renderer, NULL);

    //Get production informations
    int *production_values = get_all_inverters_production();

    //Process values
    for(int i = 0; i < number_inverters; i++){

        //Check if the production is negative or not
        if(production_values[i] < 0)
            continue;

        //Convert production from integer to string
        int size_prod = floor(log10(production_values[i])) + 2;
        char *production = NULL;
        production = malloc(size_prod*sizeof(char));
        if(production == NULL)
            report_error("Error while allocating memory !", 1);
        sprintf(production, "%d", production_values[i]);

        //Render production
        SDL_Surface *rendered_surface = NULL;
        rendered_surface = TTF_RenderText_Solid(font, production, white);

        //Transform the surface into a texture
        SDL_Texture *rendered_texture = NULL;
        rendered_texture = SDL_CreateTextureFromSurface(renderer, rendered_surface);

        //Apply the texture
        SDL_Rect destination = {WINDOW_WIDTH-rendered_surface->w, i*TTF_FontHeight(font), rendered_surface->w, rendered_surface->h};
        SDL_RenderCopy(renderer, rendered_texture, NULL, &destination);

        //Free memory
        free(production);
        SDL_FreeSurface(rendered_surface);
        SDL_DestroyTexture(rendered_texture);
    }

    //Refresh renderer
    SDL_RenderPresent(renderer);
}
