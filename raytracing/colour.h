#ifndef COLOUR_H
#define COLOUR_H

#include "vec3.h"

#include <iostream>

using colour = vec3;

inline double linearToGamma(double linearComponent)
{
    return sqrt(linearComponent);
}

void writeColor(std::ostream& out, colour pixelColour, int samplesPerPixel) {
    auto r = pixelColour.x();
    auto g = pixelColour.y();
    auto b = pixelColour.z();

    // Divide the color by the number of samples.
    auto scale = 1.0 / samplesPerPixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Apply the linear to gamma transform.
    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    // Write the translated [0,255] value of each color component.
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

#endif
