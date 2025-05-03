#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "../SDL3_gfx/SDL3_gfxPrimitives.h"
#include "map.h"

#ifndef SHAPES_H
#define SHAPES_H

#ifdef __cplusplus
extern "C" {
#endif

#define POINT_SIZE 6
#define GRID_RGBA (float[]){0.753f, 0.753f, 0.753f, 0.8f}
#define VESSEL_RGBA (float[]){0.753f, 0.753f, 0.753f, 0.8f}
#define OBSERVER_RGBA (float[]){0.753f, 0.753f, 0.753f, 0.8f}
#define MIN_GRID_SIZE 10
#define MAX_GRID_SIZE 500
#define LAT_ZONES 60
#define LON_ZONES 22

#define MAX_PATH 100

//Simplified length of a degree (at equator)

typedef struct {
  float r;
  float g;
  float b;
  float a;
}color_t;

typedef struct {
  SDL_Renderer *renderer;
  SDL_FRect    *dst_rect;
}canvas_t;

typedef struct {
  double x;
  double y;
} path_node;

typedef struct {
  int len;
  path_node nodes[MAX_PATH]; //NOTE: always allocates max path
} path_t; //TODO: dynamically allocate

typedef struct {
  //where the vessel was launched. lat/long
  point_geod start_geod;

  // where the vessel is currently at. lat/long
  point_geod pos_geod;

  point_local pos_local; //meters from origin
  //needed?
  
  //color representing this vessel. RGBA
  color_t color;

  canvas_t canvas;
} vessel_t;

// typedef enum {
//   vessel_ent,
//   observer_ent
// } entity_type;

// typedef struct {
//   float x;
//   float y;
// } point;

// typedef struct {
//   float x;
//   float y;
// } wincent_point;



// typedef enum {
//   none,
//   side_ruler,
//   grid
// } grid_options;


// void draw_entity(SDL_Renderer *renderer, int xpos, int ypos, entity_type type);
void draw_entity(SDL_Renderer *renderer, SDL_Window *window, float zoom, float xpos, float ypos);

// void draw_legend(SDL_Renderer* renderer, char *text, );

// void draw_point(point *p, Uint8 R, Uint8 G, Uint8 B, Uint8 A);

//NOTE: maybe bez the curves a lil just for show
// void draw_between_points(point *a, point *b, Uint8 R, Uint8 G, Uint8 B, Uint8 A);

// void draw_path(point *path[], Uint8 R, Uint8 G, Uint8 B, Uint8 A);

// void testing_func(SDL_Renderer *renderer);

/*
 * * scale: pixels between grid nodes
 * * NOTE: 32 bit could be suboptimal
*/
void draw_grid(SDL_Renderer *renderer, SDL_FRect *dst_rect);
void draw_grid_utm(SDL_Renderer *renderer, SDL_FRect *dst_rect);

/*
 * Usage:
 * * scale: how many meters per pixel
 * * grid_options: none, side_ruler, grid
*/
// void draw_grid(float scale, grid_options goption);
// void draw_vessel_snake(SDL_Renderer *rend, SDL_FRect *rect, vessel_t *vessel);

void draw_vessel_snake(vessel_t *vessel);

vessel_t *launch_vessel(point_geod startp, float rgba[4]);

void destroy_vessel(vessel_t *vessel);

// void move_vessel_deg(vessel_t *vessel, double move_x, double move_y);
void snake_move_vessel_m(vessel_t *vessel, point_local p);
// void move_vessel_m(vessel_t *vessel, double move_x, double move_y);
void snake_move_vessel_deg(vessel_t *vessel, point_geod p);

// void draw_path(SDL_Renderer *rend, vessel_t *vessel);
void draw_path(SDL_Renderer *rend, SDL_FRect *rect, vessel_t *vessel);

// void draw_example_point_tm(SDL_Renderer *rend, SDL_FRect *rect, double xm, double ym, tm_ellipsoid e);
void draw_example_point_tm(SDL_Renderer *rend, SDL_FRect *rect, point_geod p);
#ifdef __cplusplus
}
#endif
#endif
