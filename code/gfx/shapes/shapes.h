#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

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

#define MAX_PATH_LEN 100

//Simplified length of a degree (at equator)

double simpl_lat_to_m(double deg);
double simpl_lon_to_m(double deg);

typedef struct {
  double deg_long;//degrees
  double deg_lat;//degrees
  double len_long;//meters
  double len_lat;//meters
} FlatMap; //NOTE: Naive solution

typedef struct {
  double start_long; //degrees
  double start_lat;

  double current_x; //meters from origin
  double current_y; //meters from origin

  bool observer;

} vessel_t;

typedef enum {
  vessel_ent,
  observer_ent
} entity_type;

typedef struct {
  float x;
  float y;
} point;

typedef struct {
  float x;
  float y;
} wincent_point;



// typedef enum {
//   none,
//   side_ruler,
//   grid
// } grid_options;


// void draw_entity(SDL_Renderer *renderer, int xpos, int ypos, entity_type type);
void draw_entity(SDL_Renderer *renderer, SDL_Window *window, float zoom, float xpos, float ypos, entity_type type);

// void draw_legend(SDL_Renderer* renderer, char *text, );

void draw_point(point *p, Uint8 R, Uint8 G, Uint8 B, Uint8 A);

//NOTE: maybe bez the curves a lil just for show
void draw_between_points(point *a, point *b, Uint8 R, Uint8 G, Uint8 B, Uint8 A);

void draw_path(point *path[], Uint8 R, Uint8 G, Uint8 B, Uint8 A);

void testing_func(SDL_Renderer *renderer);

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
void draw_vessel(SDL_Renderer *rend, SDL_FRect *rect, vessel_t *vessel);

vessel_t *launch_vessel(double start_lon, double start_lat, bool observer);

void destroy_vessel(vessel_t *vessel);

void move_vessel_deg(vessel_t *vessel, double move_x, double move_y);
void move_vessel_m(vessel_t *vessel, double move_x, double move_y);

#endif
#ifdef __cplusplus
}
#endif
