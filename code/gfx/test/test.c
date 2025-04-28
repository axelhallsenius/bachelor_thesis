#include "test.h"
//TODO: introduce Google Test here

tm_ellipsoid grs80 = {
  6378137.0000,
  1/298.257222101,
};

  // 1.000002540000,
tm_grid grid = {
  // 13.58547,
  9.0,
  // 24.0,
  0.9996,
  // 1.000002540000,
  // -6226307.8640,
  0.0,
  // 84182.8790
  500000.0,
  // 0.0
};
//this actually ends up in a corner of a UTM grid

void test_geod_draw(SDL_Renderer *rend, SDL_FRect *rect){
  point_geod geop;
  geop.deg_long = 24.0;
  geop.deg_lat = 66.0;
  // gridp = geod_to_tm_grid(grs80, geop);
  draw_example_point_tm(rend, rect, geop);
}

void test_meter_to_geod(SDL_Renderer *rend, SDL_FRect *rect){
  //TODO: xy flipped
  point_tm_grid gridp;
  // gridp.x = 1135809.413803;
  // gridp.y = 555304.106555;
  // gridp.x = 0.0;
  // gridp.y = 500000.0;
  gridp.x = 5650300.787;
  gridp.y = 570168.862;
  point_geod geop = tm_grid_to_geod(grs80, grid, gridp);
  // printf("lat: %lf, long: %lf\n", geop.deg_lat, geop.deg_long);
  draw_example_point_tm(rend, rect, geop);
}

void test_geod_to_meter(SDL_Renderer *rend, SDL_FRect *rect){
  point_geod geop;
  geop.deg_lat = 59.328207;
  geop.deg_long = 14.544193;

  utm_zone z = utm_zone_from_geod(geop);

  point_tm_grid gridp = geod_to_utm_grid(geop);
  printf("x: %lf, y: %lf\n", gridp.x, gridp.y);

  point_geod newgeop = utm_grid_to_geod(gridp,z);

  // printf("oldlat: %lf, oldlong: %lf\n", newgeop.deg_lat, newgeop.deg_long);
  printf("delta lat: %lf, delta long: %lf\n", geop.deg_lat - newgeop.deg_lat, geop.deg_long - newgeop.deg_long);
  // printf("newlat: %lf, newlong: %lf\n", newgeop.deg_lat, newgeop.deg_long);

  draw_example_point_tm(rend,rect,newgeop);
}
