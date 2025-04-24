#include "test.h"

tm_ellipsoid grs80 = {
  6378137.0000,
  1/298.257222101,
  13.58547,
  1.000002540000,
  -6226307.8640,
  84182.8790
};

void test_geod_draw(SDL_Renderer *rend, SDL_FRect *rect){
  point_geod geop;
  geop.deg_long = 66.0;
  geop.deg_lat = 24.0;
  // gridp = geod_to_tm_grid(grs80, geop);
  draw_example_point_tm(rend, rect, geop);
}

// void test_
