
#pragma once

#include <algorithm>
#include <math.h>

typedef double nfloat;
using std::min;
using std::max;

constexpr nfloat deg2rad = M_PI/180.0;
constexpr nfloat rad2deg = 180.0/M_PI;

struct llh_t {nfloat lat, lon, h;};
struct ecef_t {nfloat x, y, z;};

// class WGS84 {
//   public:
namespace WGS84 {
  nfloat rf = 298.257223563;
  nfloat f = 1.0/rf;
  nfloat a = 6378137.0;
  nfloat b = a - a * f;
  nfloat e = sqrt(1.0 - ( (b*b) / (a*a) ));
  nfloat r = (2.0*a + b) / 3.0;
  nfloat rotation_period = 23*3600 + 56*60 + 4.09053;

  nfloat rate = 7.2921157e-5;  // Rotation rate of Earth [rad/s]
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
    lat = lat*deg2rad;
    nfloat G0 = 9.7803253359; // Gravity [m/sec^2]
    return G0*(1.0 + 0.0053024*pow(sin(lat),2) - 0.0000058*pow(sin(2.0*lat),2));
  }

  ecef_t llh2ecef(const llh_t& llh) {
    // matlab: https://www.mathworks.com/help/aeroblks/llatoecefposition.html
    // this works, matches: https://www.oc.nps.edu/oc2902w/coord/llhxyz.htm
    nfloat lat = llh.lat;
    nfloat lon = llh.lon;
    nfloat H = llh.h;

    nfloat mu = lat*deg2rad;
    nfloat i = lon*deg2rad;
    nfloat r = 6378137.0;
    // nfloat f = 1.0/298.257223563;
    nfloat ls = atan(pow(1.0-f,2) * tan(mu));
    nfloat rs = sqrt(r*r / (1.0 + (1.0/pow(1.0-f,2) - 1.0)*pow(sin(ls),2)));

    nfloat x = rs*cos(ls)*cos(i) + H*cos(mu)*cos(i);
    nfloat y = rs*cos(ls)*sin(i) + H*cos(mu)*sin(i);
    nfloat z = rs*sin(ls)+H*sin(mu);

    return ecef_t{x,y,z};
  }

  nfloat haversine(const llh_t& a, const llh_t& b) {
    // Returns the haversine (or great circle) distance between
    // 2 sets of GPS coordinates. This appears to work really well.
    //
    // a: (lat, lon) in deg
    // b: (lat, lon) in deg
    nfloat lat = b.lat*deg2rad;
    nfloat dlat = (b.lat - a.lat)*deg2rad;
    nfloat dlon = (b.lon - a.lon)*deg2rad;
    nfloat m = pow(sin(dlat*0.5),2) + cos(a.lat*deg2rad) * cos(b.lat*deg2rad) * pow(sin(dlon*0.5),2);
    return r*2.0*asin(min(1.0, sqrt(m)));
  }
};