#include "../../src/c/vessel.h"
#include <limits.h>

#include "gtest/gtest.h"
namespace {

TEST(Vessel, GeodeticToMeter){
  point_geod geop;
  utm_zone z = utm_zone_from_geod(geop);

  point_tm_grid gridp = geod_to_utm_grid(geop);
 
  point_geod newgeop = utm_grid_to_geod(gridp,z);

  //ankdammen utanför
  // geop.deg_lat = 59.32823000;
  // geop.deg_long = 14.54425000;

  // EXPECT_NEAR(491398.0, gridp.x, 0.5);
  // EXPECT_NEAR(5459788.0 , gridp.y, 0.5);

  geop.deg_lat = 59.32411000;
  geop.deg_long = 14.57014000;

  gridp = geod_to_utm_grid(geop);
  printf("Karlskoga Discgolf\n");
  EXPECT_NEAR(475536.0, gridp.x, 0.5);
  EXPECT_NEAR(6576222.0, gridp.y, 0.5);
  // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
  // printf("Discgo.17g:\t\t delta x: %.17g, delta y: %.17g\n", 475536.0 - gridp.x,6576222.0 - gridp.y);

  //fobollsplan vid ett raffinaderi i Abidjan, Elfenbenskusten
  geop.deg_lat = 5.25631000;
  geop.deg_long = -3.98964000;

  gridp = geod_to_utm_grid(geop);
  printf("Abidjan Football Field\n");
  EXPECT_NEAR(390332.0, gridp.x, 0.5);
  EXPECT_NEAR(581083.0, gridp.y, 0.5);
  // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
  // printf("Abidjan Football Field:\t delta x: %.17g, delta y: %.17g\n", 390332.0 - gridp.x,581083.0 - gridp.y);

  geop.deg_lat = 78.66501000;
  geop.deg_long = 16.31944000;

  gridp = geod_to_utm_grid(geop);
  printf("Schoolyard in Pyramiden, Svalbard\n");
  EXPECT_NEAR(528973.0, gridp.x, 0.5);
  EXPECT_NEAR(8731798.0 , gridp.y, 0.5);
  // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
  // printf("Schoolyard in Svalbard:\t delta x: %.17g, delta y: %.17g\n", 528973.0 - gridp.x,8731798.0 - gridp.y);

  geop.deg_lat = 49.29078000;
  geop.deg_long = -123.11829000;

  gridp = geod_to_utm_grid(geop);
  printf("Vancouver Harbour\n");
  EXPECT_NEAR(491398.0, gridp.x, 0.5);
  EXPECT_NEAR(5459788.0 , gridp.y, 0.5);
  // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
  // printf("Vancouver Harbour:\t delta x: %.17g, delta y: %.17g\n", 491398.0 - gridp.x,5459788.0 - gridp.y);
 
  geop.deg_lat = -37.84175000;
  geop.deg_long = 144.92936000;

  gridp = geod_to_utm_grid(geop);
  printf("Lighthouse in the Port of Melbourne\n");
  EXPECT_NEAR(317803.0, gridp.x, 0.5);
  EXPECT_NEAR(5809723.0, gridp.y, 0.5);
  // printf("x: %.17g, y: %.17g\n", 317803.0, 5809723.0);
  // printf("Port Melbourne Light:\t delta x: %.17g, delta y: %.17g\n", 317803.0 - gridp.x,5809723.0 - gridp.y);
}
//
// void test_geod_to_meter(SDL_Renderer *rend, SDL_FRect *rect){
//    // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
//   printf("Damm:\t\t\t delta x: %.17g, delta y: %.17g\n", 6576690.0 - gridp.x, 474066.0 - gridp.y);
//
//   printf("delta lat: %.17g, delta long: %.17g\n", geop.deg_lat - newgeop.deg_lat, geop.deg_long - newgeop.deg_long);
//
//   // printf("oldlat: %.17g, oldlong: %.17g\n", newgeop.deg_lat, newgeop.deg_long);
//   // printf("newlat: %.17g, newlong: %.17g\n", newgeop.deg_lat, newgeop.deg_long);
//
//   //discgo.17gbanan
//     draw_example_point_tm(rend,rect,geop);
// }

}
