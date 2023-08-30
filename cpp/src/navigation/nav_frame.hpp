
#pragma once

#include "common.hpp"


class NavFrame {
  public:
  NavFrame(const ecef_t& origin): origin(origin) {}

  ned_t ecef2ned(const ecef_t& ecef) { return ned_t{0,0,0}; }
  ecef_t ned2ecef(const ned_t& ned) { return ecef_t{0,0,0}; }

  protected:
  ecef_t origin;
};
