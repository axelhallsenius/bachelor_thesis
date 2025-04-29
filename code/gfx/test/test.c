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
  // printf("lat: %.17g, long: %.17g\n", geop.deg_lat, geop.deg_long);
  draw_example_point_tm(rend, rect, geop);
}

void test_geod_to_meter(SDL_Renderer *rend, SDL_FRect *rect){
  point_geod geop;
  //ankdammen utanför
  geop.deg_lat = 59.32823000;
  geop.deg_long = 14.54425000;

  utm_zone z = utm_zone_from_geod(geop);

  point_tm_grid gridp = geod_to_utm_grid(geop);
  // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
  printf("Damm:\t\t\t delta x: %.17g, delta y: %.17g\n", 6576690.0 - gridp.x, 474066.0 - gridp.y);

  point_geod newgeop = utm_grid_to_geod(gridp,z);
  printf("delta lat: %.17g, delta long: %.17g\n", geop.deg_lat - newgeop.deg_lat, geop.deg_long - newgeop.deg_long);

  // printf("oldlat: %.17g, oldlong: %.17g\n", newgeop.deg_lat, newgeop.deg_long);
  // printf("newlat: %.17g, newlong: %.17g\n", newgeop.deg_lat, newgeop.deg_long);

  //discgo.17gbanan
  geop.deg_lat = 59.32411000;
  geop.deg_long = 14.57014000;

  gridp = geod_to_utm_grid(geop);
  // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
  printf("Discgo.17g:\t\t delta x: %.17g, delta y: %.17g\n", 6576222.0 - gridp.x, 475536.0 - gridp.y);

  //fobollsplan vid ett raffinaderi i Abidjan, E.17genbenskusten
  geop.deg_lat = 5.25631000;
  geop.deg_long = -3.98964000;

  gridp = geod_to_utm_grid(geop);
  // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
  printf("Abidjan Football Field:\t delta x: %.17g, delta y: %.17g\n", 581083.0 - gridp.x, 390332.0 - gridp.y);

  geop.deg_lat = 78.66501000;
  geop.deg_long = 16.31944000;

  gridp = geod_to_utm_grid(geop);
  // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
  printf("Schoolyard in Svalbard:\t delta x: %.17g, delta y: %.17g\n", 8731798.0 - gridp.x, 528973.0 - gridp.y);

  geop.deg_lat = 49.29078000;
  geop.deg_long = -123.11829000;

  gridp = geod_to_utm_grid(geop);
  // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
  printf("Vancouver Harbour:\t delta x: %.17g, delta y: %.17g\n", 5459788.0 - gridp.x, 491398.0 - gridp.y);
 
  geop.deg_lat = -37.84175000;
  geop.deg_long = 144.92936000;

  gridp = geod_to_utm_grid(geop);
  printf("x: %.17g, y: %.17g\n", 5809723.0, 317803.0);
  printf("Port Melbourne Light:\t delta x: %.17g, delta y: %.17g\n", 5809723.0 - gridp.x, 317803.0 - gridp.y);

  draw_example_point_tm(rend,rect,newgeop);
}
