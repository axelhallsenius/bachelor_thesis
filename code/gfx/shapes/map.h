#ifndef MAP_H
#define MAP_H

#ifdef __cplusplus
extern "C" {
#endif

//Map

typedef struct {
  double deg_long;//degrees
  double deg_lat;//degrees
  double len_long;//meters
  double len_lat;//meters
} Flat_Map; //NOTE: Naive solution

typedef struct {
  double from_long;//degrees
  double from_lat;//degrees
  double to_long;//degrees
  double to_lat;//degrees
  //TODO: lots of constants go here.
} WGS_Submap;

typedef struct {
  WGS_Submap **map_matrix;
} WGS_Map;

#endif
#ifdef __cplusplus
}
#endif
