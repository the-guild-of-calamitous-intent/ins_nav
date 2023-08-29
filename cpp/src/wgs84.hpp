
#pragma once

#include <algorithm>
#include <math.h>
#include "units.hpp"

#if defined(ARDUINO)
typedef float nfloat;
#elif defined(__APPLE__) || defined(__linux__)
typedef double nfloat;
#endif

using std::min;
using std::max;

// namespace Units {
// constexpr nfloat deg2rad = M_PI/180.0;
// constexpr nfloat rad2deg = 180.0/M_PI;
// };

struct llh_t {nfloat lat, lon, h;};
struct ecef_t {nfloat x, y, z;};

// FIXME: This is a MESS ... what do I want to do?

// class WGS84 {
//   public:
namespace WGS84 {
  constexpr nfloat INV_FLATTENING     = 298.257223563;
  constexpr nfloat FLATTENING         = 1.0 / INV_FLATTENING;
  constexpr nfloat SEMI_MAJOR_AXIS_M  = 6378137.0;                  // a, m
  constexpr nfloat SEMI_MAJOR_AXIS_KM = SEMI_MAJOR_AXIS_M / 1000.0; // a, Km
  constexpr nfloat SEMI_MINOR_AXIS_M  = SEMI_MAJOR_AXIS_M - SEMI_MAJOR_AXIS_M * FLATTENING; // b, m
  // constexpr nfloat STD_PRESSURE_PA    = 101325.0;                   // Pa
  constexpr nfloat SPIN_RATE_RPS      = 7.2921150e-5;               // rad / sec
  constexpr nfloat G0                 = 9.7803253359; // Gravity [m/sec^2]

  // nfloat rf = 298.257223563;
  const nfloat f = 1.0/FLATTENING;
  const nfloat a = SEMI_MAJOR_AXIS_M;
  const nfloat b = a - a * f;
  const nfloat e = sqrt(1.0 - ( (b*b) / (a*a) ));
  const nfloat r = (2.0*a + b) / 3.0;
  const nfloat rotation_period = 23*3600 + 56*60 + 4.09053;

  nfloat rate = SPIN_RATE_RPS;  // Rotation rate of Earth [rad/s]
  nfloat sf = 1.2383e-3;       // Schuller frequency

  nfloat gravity(nfloat lat) {
    /*
    Based off the Oxford reference for the gravity formula at sealevel.
    https://www.oxfordreference.com/view/10.1093/oi/authority.20110803100007626

    Also the WGS84 has a newer model, but it is more computationally
    intensive and only differs from this one by 0.68 um/s^2
    https://en.wikipedia.org/wiki/Gravity_of_Earth

    lat: latitude [decimal deg], North is positive and South is negative
    */
    lat = lat*Units::deg2rad;
    return G0*(1.0 + 0.0053024*pow(sin(lat),2) - 0.0000058*pow(sin(2.0*lat),2));
  }

  ecef_t llh2ecef(const llh_t& llh) {
    // matlab: https://www.mathworks.com/help/aeroblks/llatoecefposition.html
    // this works, matches: https://www.oc.nps.edu/oc2902w/coord/llhxyz.htm
    // nfloat lat = llh.lat;
    // nfloat lon = llh.lon;
    const nfloat H = llh.h;

    const nfloat mu = llh.lat*Units::deg2rad;
    const nfloat i = llh.lon*Units::deg2rad;
    const nfloat r = SEMI_MAJOR_AXIS_M; //6378137.0;
    // nfloat f = 1.0/298.257223563;
    const nfloat ls = atan(pow(1.0-f,2) * tan(mu));
    const nfloat rs = sqrt(r*r / (1.0 + (1.0/pow(1.0-f,2) - 1.0)*pow(sin(ls),2)));

    const nfloat x = rs*cos(ls)*cos(i) + H*cos(mu)*cos(i);
    const nfloat y = rs*cos(ls)*sin(i) + H*cos(mu)*sin(i);
    const nfloat z = rs*sin(ls)+H*sin(mu);

    return ecef_t{x,y,z};
  }

  // llh_t ecef2llh(const ecef_t& ecef) {
  //   const double a = SEMI_MAJOR_AXIS_M;
  //   const double a2 = a*a;
  //   const double b2 = SEMI_MINOR_AXIS_M;
  //   const double e2 =
  // }

  nfloat haversine(const llh_t& a, const llh_t& b) {
    // Returns the haversine (or great circle) distance between
    // 2 sets of GPS coordinates. This appears to work really well.
    //
    // a: (lat, lon) in decimal deg
    // b: (lat, lon) in decimal deg
    const nfloat alat = a.lat*Units::deg2rad;
    const nfloat blat = b.lat*Units::deg2rad;
    const nfloat alon = a.lon*Units::deg2rad;
    const nfloat blon = b.lon*Units::deg2rad;
    const nfloat dlat = (blat - alat);
    const nfloat dlon = (blon - alon);
    const nfloat m = pow(sin(dlat*0.5),2) + cos(alat) * cos(blat) * pow(sin(dlon*0.5),2);
    return r*2.0*asin(min(1.0, sqrt(m)));
  }

  /*
  Returns the geocentric radius based on WGS84
  lat: latitude in deg
  */
  nfloat radius(float lat) {
    lat *= Units::deg2rad;
    const nfloat a = SEMI_MAJOR_AXIS_M;
    const nfloat b = a - a * FLATTENING;
    const nfloat num = pow(a * a * cos(lat), 2) + pow(b * b * sin(lat), 2);
    const nfloat den = pow(a * cos(lat), 2) + pow(b * sin(lat), 2);
    return sqrt(num / den);
  }
};
