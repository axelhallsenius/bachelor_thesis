#ifndef DRAW_H
#define DRAW_H
#ifdef __cplusplus
extern "C" {
#endif


#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "../../deps/SDL3_gfx/SDL3_gfxPrimitives.h"
#include <stdlib.h>

#include "vessel.h"

#define POINT_SIZE 6

//draw path between positions in the snake projection
void draw_path_snake(const canvas_t *canvas, const color_t *color, const SDL_FPoint *points);

//places a dot on the vessel's current coordinates
void draw_vessel_snake(canvas_t *canvas, vessel_t *vessel);

//places a dot on the vessel's current coordinates
void draw_vessel_utm(canvas_t *canvas, vessel_t *vessel);

//draws a path in UTM projection
void render_geod_path(canvas_t *canvas, point_geod *path, int len);

//draws a path in snake projection
void render_snake_path(canvas_t *canvas, point_local *path, int len);

//draws a grid that is close to UTM
void draw_grid_utm(SDL_Renderer *renderer, SDL_FRect *dst_rect);

//convert degrees on globe to a pixel position in the window
SDL_FPoint geod_to_pixels(SDL_FRect *rect, point_geod p);

//convert degrees on globe to a pixel position in the window
SDL_FPoint local_to_pixels_snake(SDL_FRect *rect, point_local p);

#ifdef __cplusplus
}
#endif
#endif
