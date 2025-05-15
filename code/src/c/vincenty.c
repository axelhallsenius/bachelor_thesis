#include "vincenty.h"
#define INV_VIN_MAX_LOOPS 50

//Using vincenty's inverse formula
//returns ellipsoidal distance
double vincenty_arc_distance(point_geod from, point_geod to){
  double phi1 = from.deg_lat;
  double phi2 = to.deg_lat;
  double lambda1 = from.deg_long;
  double lambda2 = to.deg_long;

  double U1 = atan((1 - WGS_ELSD_F) * tan(phi1));
  double U2 = atan((1 - WGS_ELSD_F) * tan(phi2));

  double lambda, omega = lambda2 - lambda1;

  double lambda_prev;

  double sinsigma;
  double cossigma;
  double tansigma;
  double sinalpha;
  double alpha;
  double cos2alpha;
  double cos2sigma_m;
  double sigma;
  for(int i = 0; i < INV_VIN_MAX_LOOPS; i++){

    sinsigma = 
      sqrt(
        pow((cos(U2) * sin(lambda)), 2) + 
        pow(
          ((cos(U1)) * sin(U2) -
          (sin(U1) * cos(U2) * cos(lambda))),
        2)
      );

    cossigma = 
      (sin(U1) * sin(U2)) + 
      (cos(U1) * cos(U2) * cos(lambda));

    tansigma = sinsigma / cossigma;

    sinalpha = (cos(U1) * cos(U2) * sin(lambda)) / sinsigma;
    alpha = asin(sinalpha);

    //cos²(alpha)
    cos2alpha = pow(cos(alpha), 2);

    //cos(2sigma_m)
    cos2sigma_m = 
      cossigma - 
      ((2 * sin(U1) * sin(U2)) / 
        cos2alpha);

    sigma = acos(cossigma);

    double C = (WGS_ELSD_F/16) * cos2alpha * (4 + WGS_ELSD_F * (4 - 3 * cos2alpha));

    lambda_prev = lambda;
    lambda = 
      omega + 
      (1 - C) * WGS_ELSD_F * sinalpha * 
        (sigma + C * sinsigma * 
          (cos2sigma_m + C * cossigma * 
            (-1 + 2 * pow(cos2sigma_m, 2))));

    if(fabs(lambda - lambda_prev) < 10e-12){
      break;
    }
  }

  double b = WGS_ELSD_A * (1 - WGS_ELSD_F);
  double upow2 = (cos2alpha * (pow(WGS_ELSD_A, 2) - pow(b, 2))) / pow(b, 2);

  double A = 1 + (upow2 / 16384) * (4096 + upow2 * (-768 + upow2 * (320 - 175 * upow2)));
  double B = (upow2 / 1024) * (256 + upow2 * (-128 + upow2 * (74 - 47 * upow2)));
  double deltasigma = B * sinsigma * 
    (cos2sigma_m +
     (B/4) * (cossigma * (-1 + 2 * cos2sigma_m)) -
     (B/6) * cos2sigma_m * (-3 + 4 * pow(sinsigma, 2) * (-3 + 4*(pow(cos2sigma_m, 2)))));

  double s = b * A * (sigma - deltasigma);

  return s;
}

//vincenty's direct formula:
//given ellipsoidal distance and geodetic azimuth alpha_{1-2} to a point s, calculates its 
point_geod vincenty_direct(point_geod from, double elp_dst, double azimuth){

  double phi1 = from.deg_lat;
  double lambda1 = from.deg_long;

  double tanU1 = (1 - WGS_ELSD_F) * tan(phi1);
  double tansigma1 = tanU1 / (cos(azimuth));

  double sinalpha = cos(VIN_U1) * sin(azimuth);

  double alpha = asin(sinalpha);

  double cos2alpha = pow(cos(alpha), 2);
  double b = VIN_b;
  double upow2 = (cos2alpha * (pow(WGS_ELSD_A, 2) - pow(b, 2))) / pow(b, 2);

  double A = VIN_A(upow2);
  double B = VIN_B(upow2);

  double sigma = (elp_dst / b * A);
  double sigma_prev;

  double twosigma_m;
  double deltasigma;

  double sigma1 = atan(tansigma1);

  double cos2sigma_m;
  for (int i = 0; i < INV_VIN_MAX_LOOPS; i++) {
    twosigma_m = 2 * sigma1 + sigma;

    cos2sigma_m = cos(twosigma_m);

    deltasigma = B * sin(sigma) * (cos2sigma_m + 
      (B/4) * 
        (cos(sigma) * 
        (-1 + 2 * pow(cos2sigma_m, 2)) -
        (B/6) * cos2sigma_m *
         (-3 + 4 * pow(sin(sigma), 2)) *
         (-3 + 4 * pow(cos2sigma_m, 2))
    ));


    sigma_prev = sigma;
    sigma = (elp_dst / (b * A)) + deltasigma;

    if (fabs(sigma - sigma_prev) < 10e-12){
      break;
    }
  }
  // twosigma_m = 2 * sigma1 + sigma;
  //
  // cos2sigma_m = cos(twosigma_m);

  double tanphi2 = VIN_TAN_PHI_2(azimuth, sigma, alpha);
  double tanlambda = (sin(sigma) * sin(azimuth)) / (cos(VIN_U1) * cos(sigma) - sin(VIN_U1) * sin(sigma) * cos(azimuth));
  double phi2 = atan(tanphi2);

  double C = (WGS_ELSD_F/16) * cos2alpha * (4 + WGS_ELSD_F * (4 - 3 * cos2alpha));

  double omega = atan(tanlambda) - (1 - C) * WGS_ELSD_F * sin(alpha) * (sigma + C * sin(sigma) * (cos2sigma_m + C * cos(sigma) * (2 * pow(cos2sigma_m, 2))));

  double lambda2 = lambda1 + omega;

  point_geod p;
  p.deg_long = to_degrees(lambda2);
  p.deg_lat = to_degrees(phi2);

  return p;
}
