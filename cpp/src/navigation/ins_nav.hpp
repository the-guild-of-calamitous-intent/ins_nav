
#pragma once

#if defined(ARDUINO)
typedef float nfloat;
#else
typedef double nfloat;
#endif

#include "common.hpp"

#ifndef UNITS_HPP
#define UNITS_HPP
    #include "units.hpp"
#endif

#include "wgs84.hpp"
#include "nav_frame.hpp"
