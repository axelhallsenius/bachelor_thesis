
#define UTM_FN_N 0.0
#define UTM_FN_S 10000000.0
#define UTM_FE 500000.0
#define UTM_CMER_SCALE 0.9996

//wgs84 ellipsoid a
#define WGS_ELSD_A 6378137.0

//wgs84 ellipsoid 1/f:
#define WGS_ELSD_F 1.0/298.257223563

#define UTM_N WGS_ELSD_F / (2.0 - WGS_ELSD_F)

#define UTM_N2 UTM_N * UTM_N
#define UTM_N3 UTM_N2 * UTM_N
#define UTM_N4 UTM_N3 * UTM_N

#define UTM_A_HAT \
    (WGS_ELSD_A / (1 + UTM_N)) * \
    (1 + (0.25 * UTM_N2) + \
    ((1.0/64.0) * UTM_N4))

#define E2 WGS_ELSD_F * (2.0 - WGS_ELSD_F)
#define D1 \
    (1.0/2.0)   * UTM_N  - \
    (2.0/3.0)   * UTM_N2 + \
    (37.0/96.0) * UTM_N3 + \
    (1.0/360.0) * UTM_N4

#define D2 \
    (1.0/48.0) * UTM_N2 + \
    (1.0/15.0) * UTM_N3 - \
    (437.0/1440.0) * UTM_N4
#define D3 \
    (17.0/480.0) * UTM_N3 - \
    (37.0/840.0) * UTM_N4

#define D4 \
    (4397.0/161280.0) * UTM_N4

#define A_STAR \
    (E2 + pow(E2, 2) + \
    pow(E2, 3) + \
    pow(E2, 4))

#define B_STAR \
    (-1.0/6.0) * (7.0 * pow(E2, 2) + \
    17.0 * pow(E2, 3) + \
    30.0 * pow(E2, 4))

#define C_STAR \
    (1.0/120.0) * (224.0 * pow(E2, 3) + \
    889.0 * pow(E2, 4))

#define D_STAR \
    (-1.0/1260.0) * 4279.0 * pow(E2, 4)

#define UTM_A E2

#define UTM_B \
    (1.0/6.0) * \
    (5 * pow(E2, 2) - \
    pow(E2, 3))

#define UTM_C \
    (1.0/120.0) * \
    (104 * pow(E2, 3) - \
    45 * pow(E2, 4))

#define UTM_D \
    (1.0/1260) * \
    (1237 * pow(E2, 4))


#define B1 \
    (1.0/2.0) * UTM_N -  \
    (2.0/3.0) * UTM_N2 +  \
    (5.0/16.0) * UTM_N3 +  \
    (41.0/180.0) * UTM_N4

#define B2 \
    (13.0/48.0) * UTM_N2 -  \
    (3.0/5.0) * UTM_N3 +  \
    (577.0/1440.0) * UTM_N4

#define B3 \
    (61.0/240.0) * UTM_N3 -  \
    (103.0/140.0) * UTM_N4

#define B4 \
    (49561.0/161280.0) * UTM_N4
