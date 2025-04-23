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

//Map

// typedef struct {
//   double len_long;//meters
//   double len_lat;//meters
// } Flat_Map; //NOTE: Naive solution

typedef enum {
  snake,
  merc,
  t_merc,
} projection_t;



/*
typedef struct {
  double max_x;
  double max_y;
  double min_x;
  double min_y;
} tm_zone;

typedef struct tm_node {
  bool isLeaf;
  int entries_amt;
  tm_zone MBRs[MAX_ENTRIES];
  struct tm_node *children[MAX_ENTRIES];
  void *data[MAX_ENTRIES];
} tm_node;

typedef struct {
  //R+tree seems the best data structure
  //key being lat long in degrees
  //A-Z s/n
  //1-60 e/w

} tm_map;


//snake projection

typedef struct {
  double from_long;//degrees
  double from_lat;//degrees
  double to_long;//degrees
  double to_lat;//degrees
  //TODO: lots of constants go here.
} WGS_Submap;

//wgs constants:

typedef struct {
  WGS_Submap **map_matrix;
} WGS_Map;
*/



#endif
#ifdef __cplusplus
}
#endif
