#include "map.h"

double to_degrees(double radians){
  return radians * (180.0 / M_PI);
}
double to_radians(double degrees){
  return degrees/180.0 * M_PI;
}

point_geod utm_grid_to_geod(point_tm_grid p, utm_zone z){
  point_geod coords;
  double fn = UTM_FN_N;
  if (z.hemisphere == HEM_S){
    fn = UTM_FN_S;
  }

  double xi = (p.y - fn) / 
    (UTM_CMER_SCALE * UTM_A_HAT);

  double eta = (p.x - UTM_FE) / 
    (UTM_CMER_SCALE * UTM_A_HAT);

  double xi_prime = xi -(
    (D1 * sin(2 * xi) * cosh(2 * eta)) - 
    (D2 * sin(4 * xi) * cosh(4 * eta)) - 
    (D3 * sin(6 * xi) * cosh(6 * eta)) - 
    (D4 * sin(8 * xi) * cosh(8 * eta)));

  double eta_prime = eta - (
    (D1 * cos(2 * xi) * sinh(2 * eta)) - 
    (D2 * cos(4 * xi) * sinh(4 * eta)) - 
    (D3 * cos(6 * xi) * sinh(6 * eta)) - 
    (D4 * cos(8 * xi) * sinh(8 * eta)));

  double conf_lat = asin(sin(xi_prime) / cosh(eta_prime));

  double delta_lambda = atan(sinh(eta_prime) / cos(xi_prime));

  double rad_lat = 
    conf_lat + sin(conf_lat) * cos(conf_lat) * (
    A_STAR + 
    (B_STAR * pow(sin(conf_lat), 2)) + 
    (C_STAR * pow(sin(conf_lat), 4)) + 
    (D_STAR * pow(sin(conf_lat), 6)));

  double lambda = to_radians(z.c_meridian) + delta_lambda;
  coords.deg_long = to_degrees(lambda);
  coords.deg_lat = to_degrees(rad_lat);

  return coords;
}

//TODO: test
utm_zone utm_zone_from_geod(point_geod p){
  utm_zone z;
  z.hemisphere = (p.deg_lat < 0);

  // zone number (1 .. 60)
  int zone_number = (int)floor((p.deg_long + 180.0)/6.0) + 1;
  if (zone_number < 1) zone_number = 1;
  if (zone_number > 60) zone_number = 60;

  // central meridian = −183deg + 6deg zone_number
  z.c_meridian = -183 + zone_number * 6;

  return z;
}

point_tm_grid geod_to_utm_grid(point_geod p){
  utm_zone zone = utm_zone_from_geod(p);
  double fn = UTM_FN_N;
  if (zone.hemisphere){
    fn = UTM_FN_S;
  }

  double phi = to_radians(p.deg_lat);
  double lambda = to_radians(p.deg_long);

  const double e2 = E2;  
  const double e4 = e2*e2, e6 = e4*e2, e8 = e4*e4;  

  const double A =  (0.5*e2 + 5.0/24*e4 + 3.0/32*e6 + 281.0/5760*e8);  
  const double B =  (5.0/48*e4 + 7.0/80*e6 + 697.0/11520*e8);  
  const double C =  (13.0/480*e6 + 461.0/13440*e8);  
  const double D =  (1237.0/161280*e8);

  double conf_lat =  
    phi  
    - A*sin(2*phi)  
    + B*sin(4*phi)  
    - C*sin(6*phi)  
    + D*sin(8*phi);  

  double delta_lambda = lambda - to_radians(zone.c_meridian);

  double xi_prime = 
    atan(tan(conf_lat) /
         cos(delta_lambda));

  double eta_prime = 
    atanh(cos(conf_lat) * 
          sin(delta_lambda));

  double sum1 = (
    (B1 * sin(2.0 * xi_prime) * cosh(2.0 * eta_prime)) + 
    (B2 * sin(4.0 * xi_prime) * cosh(4.0 * eta_prime)) + 
    (B3 * sin(6.0 * xi_prime) * cosh(6.0 * eta_prime)) + 
    (B4 * sin(8.0 * xi_prime) * cosh(8.0 * eta_prime)));

  double sum2 = (
    (B1 * cos(2.0 * xi_prime) * sinh(2.0 * eta_prime)) + 
    (B2 * cos(4.0 * xi_prime) * sinh(4.0 * eta_prime)) + 
    (B3 * cos(6.0 * xi_prime) * sinh(6.0 * eta_prime)) + 
    (B4 * cos(8.0 * xi_prime) * sinh(8.0 * eta_prime)));

  point_tm_grid coords;
  double easting = (UTM_CMER_SCALE * UTM_A_HAT * (eta_prime + sum2)) + UTM_FE;
  double northing = (UTM_CMER_SCALE * UTM_A_HAT * (xi_prime + sum1)) + fn;
  coords.x = easting;
  coords.y = northing;
  return coords;
}
