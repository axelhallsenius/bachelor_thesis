#include "map.h"

void print_geo(double deg_lat, double deg_long){
  // fmt: xx:xx:xx.xxx
}

double to_degrees(double radians){
  return radians * (180.0 / M_PI);
}
double to_radians(double degrees){
  return degrees/180.0 * M_PI;
}



//converts transverse mercator x,y to geodetic coordinates
//follows conversion from lantmäteriet
point_geod tm_grid_to_geod(tm_ellipsoid e, tm_grid g, point_tm_grid p){
  point_geod coords;

  double n = 
    e.f / 
    (2.0 - e.f);

  double a_hat = 
    (e.a / (1 + n)) * 
    (1 + 0.25*pow(n, 2) + 
    (1.0/64.0)*pow(n, 4));

  double xi = 
    (p.x - g.fn) / 
    (g.scale_factor * a_hat);

  double eta = 
    (p.y - g.fe) / 
    (g.scale_factor * a_hat);

  double d1 = 
    (1.0/2.0)*n - 
    (2.0/3.0)*pow(n, 2) + 
    (37.0/96.0)*pow(n, 3) + 
    (1.0/360.0)*pow(n, 4);
  
  double d2 = 
    (1.0/48.0)*pow(n, 2) + 
    (1.0/15.0)*pow(n, 3) - 
    (437.0/1440.0)*pow(n, 4);
  
  double d3 = 
    (17.0/480.0)*pow(n, 3) - 
    (37.0/840.0)*pow(n, 4);

  double d4 = 
    (4397.0/161280.0)*pow(n, 4);


  double xi_prime = xi -(
    d1*sin(2*xi)*cosh(2*eta) - 
    d2*sin(4*xi)*cosh(4*eta) - 
    d3*sin(6*xi)*cosh(6*eta) - 
    d4*sin(8*xi)*cosh(8*eta));

  double eta_prime = eta - (
    d1*cos(2*xi)*sinh(2*eta) - 
    d2*cos(4*xi)*sinh(4*eta) - 
    d3*cos(6*xi)*sinh(6*eta) - 
    d4*cos(8*xi)*sinh(8*eta));

  double conf_lat = asin(sin(xi_prime) / cosh(eta_prime));

  double delta_lambda = atan(sinh(eta_prime) / cos(xi_prime));

  double e_squared = e.f * (2.0 - e.f);

  double A_star = 
    (e_squared + pow(e_squared, 2) + 
    pow(e_squared, 3) + 
    pow(e_squared, 4));

  double B_star = 
    -1.0/6.0 * (7.0*pow(e_squared, 2) + 
    17.0*pow(e_squared, 3) + 
    30.0*pow(e_squared, 4));

  double C_star = 
    1.0/120.0 * (224.0*pow(e_squared, 3) + 
    889.0*pow(e_squared, 4));

  double D_star = 
    -1.0/1260.0 * 4279.0*pow(e_squared, 4);

  double rad_lat = 
    conf_lat + sin(conf_lat) * cos(conf_lat) * (
    A_star + 
    B_star*pow(sin(conf_lat), 2) + 
    C_star*pow(sin(conf_lat), 4) + 
    D_star*pow(sin(conf_lat), 6));


  double lambda = to_radians(g.central_mer) + delta_lambda;
  coords.deg_long = to_degrees(lambda);
  coords.deg_lat = to_degrees(rad_lat);

  return coords;
}


//TODO: predefine constants
//TODO: test
point_tm_grid geod_to_tm_grid(tm_ellipsoid e, tm_grid g, point_geod p){
  double A = e.f*(2 - e.f);

  double B = 
    (1.0/6.0) * 
    (5*pow(A, 2) - 
    pow(A, 3));

  double C = 
    (1.0/120.0) * 
    (104*pow(A, 3) - 
    45*pow(A, 4));

  double D = 
    (1.0/1260) * 
    (1237*pow(A, 4));

  double phi = to_radians(p.deg_lat);
  double lambda = to_radians(p.deg_long);

  double n = e.f / (2 - e.f);

  double a_hat = 
    (e.a / (1 + n)) * 
    (1.0 + (1.0/4.0)*pow(n, 2) + 
    (1.0/64)*pow(n, 4));

  double conf_lat = 
    phi - 
    sin(phi) * cos(phi) * (A + B*pow(sin(phi),2) + 
    C*pow(sin(phi),4) + 
    D*pow(sin(phi), 6));

  double delta_lambda = p.deg_long - to_radians(g.central_mer);

  double xi_prime = 
    atan(tan(conf_lat) /
         cos(delta_lambda));

  double eta_prime = 
    atanh(cos(conf_lat) * 
          sin(delta_lambda));

  double b1 = 
    (1.0/2.0)*n - 
    (2.0/3.0)*pow(n, 2) + 
    (5.0/16.0)*pow(n, 3) + 
    (41.0/180.0)*pow(n, 4);

  double b2 = 
    (13.0/48.0)*pow(n, 2) - 
    (3.0/5.0)*pow(n, 3) + 
    (577.0/1440.0)*pow(n, 4);

  double b3 = 
    (61.0/240.0)*pow(n, 3) - 
    (103.0/140.0)*pow(n, 4);

  double b4 = 
    (49561.0/161280.0)*pow(n, 4);

  double sum1 = (
    b1*sin(2*xi_prime)*cosh(2*eta_prime) + 
    b2*sin(4*xi_prime)*cosh(4*eta_prime) + 
    b3*sin(6*xi_prime)*cosh(6*eta_prime) + 
    b4*sin(8*xi_prime)*cosh(8*eta_prime));

  double sum2 = (
    b1*cos(2*xi_prime)*sinh(2*eta_prime) + 
    b2*cos(4*xi_prime)*sinh(4*eta_prime) + 
    b3*cos(6*xi_prime)*sinh(6*eta_prime) + 
    b4*cos(8*xi_prime)*sinh(8*eta_prime));

  point_tm_grid coords;
  coords.x = g.scale_factor * a_hat * (xi_prime + sum1) + g.fn;
  coords.y = g.scale_factor * a_hat * (eta_prime + sum2) + g.fe;
  return coords;
}

point_geod utm_grid_to_geod(point_tm_grid p, utm_zone z){
  point_geod coords;
  double fn = UTM_FN_N;
  if (z.hemisphere){
    fn = UTM_FN_S;
  }

  double xi = 
    (p.x - fn) / 
    (UTM_CMER_SCALE * UTM_A_HAT);

  double eta = 
    (p.y - UTM_FE) / 
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
