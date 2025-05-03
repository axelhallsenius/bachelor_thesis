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
void snake_draw_path(vessel_t *vessel, point_local old);

//draws a dot of the vessel position in the snake projection
void draw_vessel_snake(vessel_t *vessel);

//draws a grid
void draw_grid(SDL_Renderer *renderer, SDL_FRect *dst_rect);

//draws a grid that is close to UTM
void draw_grid_utm(SDL_Renderer *renderer, SDL_FRect *dst_rect);

//used by test functions to draw example points
void draw_example_point_tm(SDL_Renderer *rend, SDL_FRect *rect, point_geod p);

/*
 * / Functions
*/

#ifdef __cplusplus
}
#endif
#endif
