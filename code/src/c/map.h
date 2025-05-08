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
#define HEM_S 1
#define HEM_N 0

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int hemisphere; //north is 0 and south is 1
  int c_meridian; //central meridian of the zone (in degrees from the prime meridian)
} utm_zone;

typedef struct {
  double a; //semi major axis
  double f_inv; // 1/f : flattening factor
  double gm; //geocentric gravitational constant m³/s²
  double omega; // rad/s
} wgs_ellipsoid;

typedef struct {
  double a; //semi major axis
  double f; // 1/f : flattening factor
} tm_ellipsoid;

typedef struct{
  double deg_lat;
  double deg_long;
} point_geod;

typedef struct {
  double x;
  double y;
} point_local;

typedef struct{
  double central_mer; //degrees east to the centerpoint of the grid
  double scale_factor; //scale factor on the central meridian of the grid
  double fn; //False Northing
  double fe; //False easting 
} tm_grid;

typedef struct{
  double x;
  double y;
} point_tm_grid;

//aquires the utm_zone from a lat/long coordinate
utm_zone utm_zone_from_geod(point_geod p);

//converts a point in a zone to lat/long
point_geod utm_grid_to_geod(point_tm_grid p, utm_zone z);

//converts lat/long into a point in a grid
point_tm_grid geod_to_utm_grid(point_geod p);
#ifdef __cplusplus
}
#endif
#endif
