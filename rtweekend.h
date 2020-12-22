#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utitlity functions
inline double deg2Rad(double deg) { return deg * pi / 180.0; }

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Random nunmbers
inline double randomDouble() {
    // Returns a random real in [0,1).
    // RAND_MAX is some large number that can vary, but is >32767
    return rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*randomDouble();
}

// Common headers
#include "ray.h"
#include "vec3.h"
#include "hittable.h"
#include "material.h"

#endif