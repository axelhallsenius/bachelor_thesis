#include "../src/c/vessel.h"
#include <limits.h>

#include "gtest/gtest.h"
namespace {

// TEST(Vessel, GeodeticToMeter){
  // point_geod geop;
  // utm_zone z = utm_zone_from_geod(geop);

  // point_tm_grid gridp = geod_to_utm_grid(geop);
 
  // point_geod newgeop = utm_grid_to_geod(gridp,z);

  //ankdammen utanför
  // geop.deg_lat = 59.32823000;
  // geop.deg_long = 14.54425000;

  // EXPECT_NEAR(491398.0, gridp.x, 0.5);
  // EXPECT_NEAR(5459788.0 , gridp.y, 0.5);

TEST(GeodeticToMeter, KarlskogaDiscgolf){
  point_geod geop;
  point_tm_grid gridp;
  geop.deg_lat = 59.32411000;
  geop.deg_long = 14.57014000;

  gridp = geod_to_utm_grid(geop);
  EXPECT_NEAR(475536.0, gridp.x, 0.5);
  EXPECT_NEAR(6576222.0, gridp.y, 0.5);
}
  // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
  // printf("Discgo.17g:\t\t delta x: %.17g, delta y: %.17g\n", 475536.0 - gridp.x,6576222.0 - gridp.y);

  //fobollsplan vid ett raffinaderi i Abidjan, Elfenbenskusten
TEST(GeodeticToMeter, AbidjanFootballField){
  point_geod geop;
  point_tm_grid gridp;
  geop.deg_lat = 5.25631000;
  geop.deg_long = -3.98964000;

  gridp = geod_to_utm_grid(geop);
  // printf("Abidjan Football Field\n");
  EXPECT_NEAR(390332.0, gridp.x, 0.5);
  EXPECT_NEAR(581083.0, gridp.y, 0.5);
  // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
  // printf("Abidjan Football Field:\t delta x: %.17g, delta y: %.17g\n", 390332.0 - gridp.x,581083.0 - gridp.y);
}
//
// TEST(GeodeticToMeter, SchoolyardSvalbard){
//   point_geod geop;
//   point_tm_grid gridp;
//   geop.deg_lat = 78.66501000;
//   geop.deg_long = 16.31944000;
//
//   gridp = geod_to_utm_grid(geop);
//   // printf("Schoolyard in Pyramiden, Svalbard\n");
//   EXPECT_NEAR(528973.0, gridp.x, 0.5);
//   EXPECT_NEAR(8731798.0 , gridp.y, 0.5);
//   // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
//   // printf("Schoolyard in Svalbard:\t delta x: %.17g, delta y: %.17g\n", 528973.0 - gridp.x,8731798.0 - gridp.y);
//   }
//
TEST(GeodeticToMeter, VancouverHarbour){
  point_geod geop;
  point_tm_grid gridp;
  geop.deg_lat = 49.29078000;
  geop.deg_long = -123.11829000;

  gridp = geod_to_utm_grid(geop);
  // printf("Vancouver Harbour\n");
  EXPECT_NEAR(491398.0, gridp.x, 0.5);
  EXPECT_NEAR(5459788.0 , gridp.y, 0.5);
  // printf("x: %.17g, y: %.17g\n", gridp.x, gridp.y);
  // printf("Vancouver Harbour:\t delta x: %.17g, delta y: %.17g\n", 491398.0 - gridp.x,5459788.0 - gridp.y);
  }
 
TEST(GeodeticToMeter, PortOfMelbourne){
  point_geod geop;
  point_tm_grid gridp;
  geop.deg_lat = -37.84175000;
  geop.deg_long = 144.92936000;

  gridp = geod_to_utm_grid(geop);
  // printf("Lighthouse in the Port of Melbourne\n");
  EXPECT_NEAR(317803.0, gridp.x, 0.5);
  EXPECT_NEAR(5809723.0, gridp.y, 0.5);
}

 
// TODO: write about accuracy of wgs projection
// TODO: redo other tests with data from here.
// https://www.ngs.noaa.gov/NCAT/
TEST(GeodeticToMeter, Dammbron){ //sub cm accuracy
  point_geod geop;
  point_tm_grid ans;
  point_tm_grid gridp;
  geop.deg_lat = 59.3281026485;
  geop.deg_long = 14.5440262556;
  ans.x = 474053.042;
  ans.y = 6576676.167;

  gridp = geod_to_utm_grid(geop);
  EXPECT_NEAR(ans.x, gridp.x, 0.01);
  EXPECT_NEAR(ans.y, gridp.y, 0.01);
}

TEST(MeterToGeodetic, AngstromCrossroad){
  point_geod geop;
  point_geod ans;
  ans.deg_long = 17.6447886229;
  ans.deg_lat = 59.8399342516;
  point_tm_grid gridp;

  utm_zone zone = utm_zone_from_geod(ans);

  gridp.y = 6636543.595;
  gridp.x = 648206.154;
  geop = utm_grid_to_geod(gridp, zone);

  EXPECT_NEAR(ans.deg_long, geop.deg_long, 0.005);
  EXPECT_NEAR(ans.deg_lat, geop.deg_lat, 0.005);
}

TEST(MeterToGeodetic, NullIsland){
  point_geod geop;
  point_geod ans;
  ans.deg_long = 0.0;
  ans.deg_lat = 0.0;
  point_tm_grid gridp;

  utm_zone zone = utm_zone_from_geod(ans);

  gridp.y = 0.0;
  gridp.x = 833978.557;
  geop = utm_grid_to_geod(gridp, zone);

  EXPECT_NEAR(ans.deg_long, geop.deg_long, 0.005);
  EXPECT_NEAR(ans.deg_lat, geop.deg_lat, 0.005);
}

TEST(MeterToGeodetic, MuseumSantiagoChile){
  point_geod geop;
  point_geod ans;
  ans.deg_long = -70.6506621838;
  ans.deg_lat = -33.4371849463;
  point_tm_grid gridp;

  utm_zone zone = utm_zone_from_geod(ans);

  gridp.y = 6299026.868;
  gridp.x = 346558.574;
  geop = utm_grid_to_geod(gridp, zone);

  EXPECT_NEAR(ans.deg_long, geop.deg_long, 0.005);
  EXPECT_NEAR(ans.deg_lat, geop.deg_lat, 0.005);
}

}
