#include "map.h"



void print_geo(double deg_lat, double deg_long){
  // fmt: xx:xx:xx.xxx
}

//geodetic coordinates
typedef struct{
  double lats;
  double longs;
} point_geod;

typedef struct{
  double x;
  double y;
} point_tm_grid;


//GRS 1980 from lantmäteriet 
tm_ellipsoid grs_1980 = {
  6378137.0000,
  1/298.257222101,
  13.58547,
  1.000002540000,
  -6226307.8640,
  84182.8790
};

//converts transverse mercator x,y to geodetic coordinates
//follows conversion from lantmäteriet
point_geod tm_grid_to_geod(point_tm_grid p){
  point_geod coords;
  double xi;
  double eta;
  
  //TODO: not yet implemented

  return coords;
}

point_tm_grid geod_to_tm_grid(point_geod p){
  point_tm_grid coords;
  //double conformal_lat = 
  return coords;
}

