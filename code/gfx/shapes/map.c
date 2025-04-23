#include "map.h"



void print_geo(double deg_lat, double deg_long){
  // fmt: xx:xx:xx.xxx
}

//geodetic coordinates
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
point_geod tm_grid_to_geod(tm_ellipsoid e, point_tm_grid p){
  point_geod coords;
  double n = e.f / (2 - e.f);
  double a_hat = (e.a / (1 + n)) * (1 + (1.0/4.0)*pow(n, 2) + (1/64)*pow(n, 4));
  double xi = (p.x - e.false_northing) / (e.central_meridian_scale_factor * a_hat);
  double eta = (p.y - e.false_easting) / (e.central_meridian_scale_factor * a_hat);
  //TODO: not yet implemented

  return coords;
}

//TODO: predefine constants
point_tm_grid geod_to_tm_grid(tm_ellipsoid e, point_geod p){
  double A = e.f*(2 - e.f);
  double B = (1.0/6.0) * (5*pow(A, 2) - pow(A, 3));
  double C = (1.0/120.0) * (104*pow(A, 3) - 45*pow(A, 4));
  double D = (1.0/1260) * (1237*pow(A, 4));
  double phi = p.deg_lat;
  double n = e.f / (2 - e.f);
  double a_hat = (e.a / (1 + n)) * (1 + (1.0/4.0)*pow(n, 2) + (1/64)*pow(n, 4));

  double conf_lat = phi - sin(phi)*cos(phi)*(A + B*pow(sin(phi),2) + C*pow(sin(phi),4) + D*pow(sin(phi), 6));

  double delta_lambda = p.deg_long - e.central_meridian_longitude;
  double xi_prime = atan(tan(conf_lat)/cos(delta_lambda));
  double eta_prime = atanh(cos(conf_lat) * sin(delta_lambda));

  double b1 = (1.0/2.0)*n - (2.0/3.0)*pow(n, 2) + (5.0/16.0)*pow(n, 3) + (41.0/180.0)*pow(n, 4);
  double b2 = (13.0/48.0)*pow(n, 2) - (3.0/5.0)*pow(n, 3) + (577.0/1440.0)*pow(n, 4);
  double b3 = (61.0/240.0)*pow(n, 3) - (103.0/140.0)*pow(n, 4);
  double b4 = (49561.0/161280.0)*pow(n, 4);

  double sum1 = (b1*sin(2*xi_prime)*cosh(2*eta_prime) + b2*sin(4*xi_prime)*cosh(4*eta_prime) + b3*sin(6*xi_prime)*cosh(6*eta_prime) + b4*sin(8*xi_prime)*cosh(8*eta_prime));

  double sum2 = (b1*cos(2*xi_prime)*sinh(2*eta_prime) + b2*cos(4*xi_prime)*sinh(4*eta_prime) + b3*cos(6*xi_prime)*sinh(6*eta_prime) + b4*cos(8*xi_prime)*sinh(8*eta_prime));

  point_tm_grid coords;
  coords.x = e.central_meridian_scale_factor * a_hat * (xi_prime + sum1) + e.false_northing;
  coords.y = e.central_meridian_scale_factor * a_hat * (eta_prime + sum2) + e.false_easting;
  return coords;
}

