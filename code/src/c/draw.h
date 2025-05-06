#ifndef DRAW_H
#define DRAW_H
#ifdef __cplusplus
extern "C" {
#endif


#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "../../deps/SDL3_gfx/SDL3_gfxPrimitives.h"

#include "vessel.h"

#define POINT_SIZE 6

/*
 * Functions:
*/


//draw path between positions in the snake projection
// void draw_path_snake(canvas_t canvas, color_t color, path_t path);
void draw_path_snake(const canvas_t *canvas, const color_t *color, const SDL_FPoint *points);

//draws a dot of the vessel position in the snake projection
void draw_vessel_snake(vessel_t *vessel);

SDL_FPoint local_to_pixels_snake(SDL_FRect *rect, point_local p);
//draws a grid
void draw_grid(SDL_Renderer *renderer, SDL_FRect *dst_rect);

//draws a grid that is close to UTM
void draw_grid_utm(SDL_Renderer *renderer, SDL_FRect *dst_rect);

//used by test functions to draw example points
void draw_example_point_tm(SDL_Renderer *rend, SDL_FRect *rect, point_geod p);

//convert degrees on globe to a pixel position in the window
// void geod_to_pixels(SDL_FRect *rect, point_geod p, double *x, double *y);
SDL_FPoint geod_to_pixels(SDL_FRect *rect, point_geod p);


//convert degrees on globe to a pixel position in the window
SDL_FPoint local_to_pixels_snake(SDL_FRect *rect, point_local p);

/*
 * / Functions
*/

#ifdef __cplusplus
}
#endif
#endif
