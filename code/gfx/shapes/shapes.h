#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#ifndef SHAPES_H
#define SHAPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  float x;
  float y;
} point;

// typedef enum {
//   none,
//   side_ruler,
//   grid
// } grid_options;

// void draw_point(point *p, ImVec4 color);

//NOTE: maybe bez the curves a lil just for show
// void draw_between_points(point *a, point *b, ImVec4 color);

// void draw_path(point *path[], ImVec4 color);

void testing_func(SDL_Renderer *renderer);

/*
 * Usage:
 * * scale: how many meters per pixel
 * * grid_options: none, side_ruler, grid
*/
// void draw_grid(float scale, grid_options goption);


#endif
#ifdef __cplusplus
}
#endif
