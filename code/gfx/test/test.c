#include "test.h"
//TODO: introduce Google Test here

tm_ellipsoid grs80 = {
  6378137.0000,
  1/298.257222101,
};

  // 1.000002540000,
tm_grid grid = {
  13.58547,
  // 66.0,
  // 0.9996,
  1.000002540000,
  -6226307.8640,
  84182.8790
  // 0.0,
  // 0.0
};
//this actually ends up in a corner of a UTM grid

void test_geod_draw(SDL_Renderer *rend, SDL_FRect *rect){
  point_geod geop;
  geop.deg_long = 66.0;
  geop.deg_lat = 24.0;
  // gridp = geod_to_tm_grid(grs80, geop);
  draw_example_point_tm(rend, rect, geop);
}

void test_meter_to_geod(SDL_Renderer *rend, SDL_FRect *rect){
  point_tm_grid gridp;
  gridp.x = 1135809.413803;
  gridp.y = 555304.106555;
  // gridp.x = 0.0;
  // gridp.y = 0.0;
  point_geod geop = tm_grid_to_geod(grs80, grid, gridp);
  printf("lat: %lf, long: %lf\n", geop.deg_lat, geop.deg_long);
  draw_example_point_tm(rend, rect, geop);
}
