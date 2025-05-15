#include "ellipsoid.h"
#include "map.h"

#ifndef VINCENTY_H
#define VINCENTY_H

#ifdef __cplusplus
extern "C" {
#endif

double vincenty_arc_distance(point_geod from, point_geod to);

point_geod vincenty_direct(point_geod from, double elp_dst, double azimuth);

#ifdef __cplusplus
}
#endif
#endif
