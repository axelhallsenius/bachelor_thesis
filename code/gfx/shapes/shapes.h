#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#ifndef SHAPES_H
#define SHAPES_H

#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
  vessel,
  observer
} entity_type;

typedef struct {
  float x;
  float y;
} point;

// typedef enum {
//   none,
//   side_ruler,
//   grid
// } grid_options;


void draw_entity(SDL_Renderer *renderer, int xpos, int ypos, entity_type type);

// void draw_legend()

void draw_point(point *p, Uint8 R, Uint8 G, Uint8 B, Uint8 A);

//NOTE: maybe bez the curves a lil just for show
void draw_between_points(point *a, point *b, Uint8 R, Uint8 G, Uint8 B, Uint8 A);

void draw_path(point *path[], Uint8 R, Uint8 G, Uint8 B, Uint8 A);

void testing_func(SDL_Renderer *renderer);

/*
 * * scale: pixels between grid nodes
 * * NOTE: 32 bit could be suboptimal
*/
void draw_grid(SDL_Renderer *renderer, SDL_Window *window, Uint32 scale);

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
