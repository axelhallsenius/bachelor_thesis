#define MAX_ENTRIES 4
#include <stdbool.h>
#include <math.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


#ifndef MAP_H
#define MAP_H

//definitions from lantmäteriet
//false northing
// #define FN -6626307.8640
// //false easting
// #define FE 84182.8790
// //scale factor on the central meridian
// #define SCALE_C_MER 1.000002540000
//
// #define SCALE_C_MER 1.000002540000

#ifdef __cplusplus
extern "C" {
#endif


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
  double central_meridian_longitude;
  double central_meridian_scale_factor;
  double false_northing; // rad/s
  double false_easting; // rad/s
} tm_ellipsoid;

typedef enum {
  snake,
  t_merc,
  wgs84
} projection_t;

typedef struct{
  double deg_lat;
  double deg_long;
} point_geod;

//NOTE: how to choose this grid?
typedef struct{
  //points outside the grid to prevent numbers in grid from becoming negative
  double fe; //False easting 
  double fn; //False Northing
  double central_mer; //degrees east to the centerpoint of the grid
  double scale_factor; //scale factor on the central meridian of the grid
} tm_grid;


typedef struct{
  double x;
  double y;
} point_tm_grid;

point_tm_grid geod_to_tm_grid(tm_ellipsoid e, point_geod p);

point_geod tm_grid_to_geod(tm_ellipsoid e, point_tm_grid p);

// void test_geod_grid();

#ifdef __cplusplus
}
#endif
#endif
