#define MAX_ENTRIES 4
#define EARTH_CIRC_EQ 40075016.0
#define EARTH_POLAR_DIST 20004000.0
#include <stdbool.h>
#include <math.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "ellipsoid.h"
#include <stdio.h>

#ifndef MAP_H
#define MAP_H
//central_mer is always +3 deg from edge of zone

//definitions from lantmäteriet
//false northing
// #define FN -6626307.8640
// //false easting
// #define FE 84182.8790
// //scale factor on the central meridian
// #define SCALE_C_MER 1.000002540000
//
// #define SCALE_C_MER 1.000002540000
#define HEM_S 1
#define HEM_N 0

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int hemisphere;
  int c_meridian;
} utm_zone;

typedef struct {
  double a; //semi major axis
  double f_inv; // 1/f : flattening factor
  double gm; //geocentric gravitational constant m³/s²
  double omega; // rad/s
  // double 
} wgs_ellipsoid;

typedef struct {
  double a; //semi major axis
  double f; // 1/f : flattening factor
  // double central_meridian_longitude;
  // double central_meridian_scale_factor;
  // double false_northing; // rad/s
  // double false_easting; // rad/s
} tm_ellipsoid;
//
// typedef enum {
//   snake,
//   t_merc,
//   wgs84
// } projection_t;
//
typedef struct{
  double deg_lat;
  double deg_long;
} point_geod;

typedef struct {
  double x;
  double y;
} point_local;

//NOTE: how to choose this grid?
typedef struct{
  double central_mer; //degrees east to the centerpoint of the grid
  double scale_factor; //scale factor on the central meridian of the grid
  //points outside the grid to prevent numbers in grid from becoming negative
  double fn; //False Northing
  double fe; //False easting 
} tm_grid;

typedef struct{
  double x;
  double y;
} point_tm_grid;

point_tm_grid geod_to_tm_grid(tm_ellipsoid e, tm_grid g, point_geod p);

point_geod tm_grid_to_geod(tm_ellipsoid e, tm_grid g, point_tm_grid p);

// void test_geod_grid();

void snake_local_to_pixels(SDL_FRect *rect, point_local p);

utm_zone utm_zone_from_geod(point_geod p);
point_geod utm_grid_to_geod(point_tm_grid p, utm_zone z);
point_tm_grid geod_to_utm_grid(point_geod p);
#ifdef __cplusplus
}
#endif
#endif
