#define MAX_ENTRIES 4
#include <stdbool.h>
#include <math.h>


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
  merc,
  t_merc,
} projection_t;

typedef struct{
  double deg_lat;
  double deg_long;
} point_geod;

typedef struct{
  double x;
  double y;
} point_tm_grid;

point_tm_grid geod_to_tm_grid(tm_ellipsoid e, point_geod p);


// void test_geod_grid();

#ifdef __cplusplus
}
#endif
#endif
