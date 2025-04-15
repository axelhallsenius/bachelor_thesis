#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#ifndef TRANSLATION_H
#define TRANSLATION_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  float x;
  float y;
} win_coords;

typedef struct {
  double lat;
  double lon;
} lat_lon_snake;

typedef struct {
  double x;
  double y;
} vessel_start_coords;

/*
 * Convert window centric coordinates (origin is center) 
 * to window printing coordinates (origin is top left)
*/
win_coords cent_to_win(SDL_Window *window, float x, float y);


/*
 * Movement over the edges will modulo
 * 
*/
// void move_point_snake(int x_max, int y_max, int x, int y);

/*
 * 
 * 
*/
win_coords deg_to_win_snake(SDL_Window *window, lat_lon_snake *deg);


#endif
#ifdef __cplusplus
}
#endif
