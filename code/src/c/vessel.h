#include "map.h"

#ifndef SHAPES_H
#define SHAPES_H

#ifdef __cplusplus
extern "C" {
#endif

#define GRID_RGBA (float[]){0.753f, 0.753f, 0.753f, 0.8f}
#define VESSEL_RGBA (float[]){0.753f, 0.753f, 0.753f, 0.8f}
#define OBSERVER_RGBA (float[]){0.753f, 0.753f, 0.753f, 0.8f}
#define MIN_GRID_SIZE 10
#define MAX_GRID_SIZE 500
#define LAT_ZONES 60
#define LON_ZONES 22

#define MAX_PATH 100

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
  int len;
  point_local *deltas;
} move_order_t;

typedef struct {
  //where the vessel was launched. lat/long
  //needed?
  point_geod start_geod;

  // where the vessel is currently at. lat/long
  point_geod pos_geod;
  point_local pos_local_utm;
  utm_zone zone;

  point_local start_snake;
  point_local pos_snake;

  int steps_snake;
  int steps_utm;

} vessel_t;


// void draw_entity(SDL_Renderer *renderer, int xpos, int ypos, entity_type type);
// void draw_entity(SDL_Renderer *renderer, SDL_Window *window, float zoom, float xpos, float ypos);

// void draw_legend(SDL_Renderer* renderer, char *text, );

// void draw_point(point *p, Uint8 R, Uint8 G, Uint8 B, Uint8 A);

//NOTE: maybe bez the curves a lil just for show
// void draw_between_points(point *a, point *b, Uint8 R, Uint8 G, Uint8 B, Uint8 A);

// void draw_path(point *path[], Uint8 R, Uint8 G, Uint8 B, Uint8 A);

// void testing_func(SDL_Renderer *renderer);

// void draw_grid(SDL_Renderer *renderer, SDL_FRect *dst_rect);
// void draw_grid_utm(SDL_Renderer *renderer, SDL_FRect *dst_rect);

/*
 * Usage:
 * * scale: how many meters per pixel
 * * grid_options: none, side_ruler, grid
*/
// void draw_grid(float scale, grid_options goption);
// void draw_vessel_snake(SDL_Renderer *rend, SDL_FRect *rect, vessel_t *vessel);

// void draw_vessel_snake(vessel_t *vessel);

// move_order_t *create_random_move_order(int len, int scale);

// vessel_t *launch_vessel(canvas_t *canvas, point_geod startp, float rgba[4]);
vessel_t *launch_vessel(point_geod startp, int path_len);

void destroy_vessel(vessel_t *vessel);

point_local *make_path_snake(point_geod start, move_order_t *order);
point_geod *make_path_utm(point_geod start, move_order_t *order);
// void move_vessel_snake(vessel_t *vessel, point_local delta);
// void move_vessel_snake(vessel_t *vessel, point_local delta, point_geod geod_path[]);
void move_vessel_snake(vessel_t *vessel, point_local delta);
void move_vessel_utm(vessel_t *vessel, point_local delta);
// void move_vessel_utm(vessel_t *vessel, point_local delta);
// void move_vessel_deg(vessel_t *vessel, double move_x, double move_y);
point_local snake_move_vessel_m(vessel_t *vessel, point_local p);
// void move_vessel_m(vessel_t *vessel, double move_x, double move_y);
void snake_move_vessel_deg(vessel_t *vessel, point_geod p);

// void draw_path(SDL_Renderer *rend, vessel_t *vessel);
// void draw_path(SDL_Renderer *rend, SDL_FRect *rect, vessel_t *vessel);

// void draw_example_point_tm(SDL_Renderer *rend, SDL_FRect *rect, double xm, double ym, tm_ellipsoid e);
void draw_example_point_tm(SDL_Renderer *rend, SDL_FRect *rect, point_geod p);

// move_order_t create_random_move_order(int len, int scale);
move_order_t *create_random_move_order(int len, int scale);
void update_vessel_pos(vessel_t *vessel, point_geod g, point_local l);

void destroy_move_order(move_order_t *order);
#ifdef __cplusplus
}
#endif
#endif
