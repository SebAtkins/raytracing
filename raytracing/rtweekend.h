#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>


// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

inline double randomDouble() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * randomDouble();
}

// Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include "colour.h"
#include "material.h"

#endif
