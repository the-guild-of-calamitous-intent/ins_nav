
#pragma once

#if defined(ARDUINO)
typedef float nfloat;
#elif defined(__APPLE__) || defined(__linux__)
typedef double nfloat;
#endif

#include "common.hpp"

#ifndef UNITS_HPP
#define UNITS_HPP
    #include "units.hpp"
#endif

#include "wgs84.hpp"
#include "nav_frame.hpp"
