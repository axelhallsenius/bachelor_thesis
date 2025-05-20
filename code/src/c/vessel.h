#include "map.h"

#ifndef SHAPES_H
#define SHAPES_H

#ifdef __cplusplus
extern "C" {
#endif

#define GRID_RGBA (float[]){0.753f, 0.753f, 0.753f, 0.8f}
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

} vessel_t;

vessel_t *launch_vessel(point_geod startp);


move_order_t *create_random_move_order(int len, int scale);
void destroy_move_order(move_order_t *order);
void update_vessel_pos(vessel_t *vessel, point_geod g, point_local l);

void set_vessel_start(vessel_t *vessel, point_geod startp);
point_local *make_path_snake(point_local start, move_order_t *order);
point_geod *make_path_utm(point_geod start, move_order_t *order);

#ifdef __cplusplus
}
#endif
#endif
