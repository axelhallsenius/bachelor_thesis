#define MAX_ENTRIES 4
#include <stdbool.h>

#ifndef MAP_H
#define MAP_H

#ifdef __cplusplus
extern "C" {
#endif

//Map

// typedef struct {
//   double len_long;//meters
//   double len_lat;//meters
// } Flat_Map; //NOTE: Naive solution



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

#endif
#ifdef __cplusplus
}
#endif
