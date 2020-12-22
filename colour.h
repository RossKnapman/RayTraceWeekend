#ifndef COLOUR_H
#define COLOUR_H

#include "rtweekend.h"

#include <iostream>

// static_cast<TYPE> performs an explicit type conversion
// It is better than (int)x as the compiler will spot it if it won't work and raise an error (so won't break at runtime instead)

void writeColour(std::ostream &out, colour pixelColour, int samplesPerPixel) {
    // Write out the translated [0, 255] value of each colour component

    auto r = pixelColour.x();
    auto g = pixelColour.y();
    auto b = pixelColour.z();

    // Divide the colour by the number of samples and gamma-correct for gamma = 2.0
    auto scale = 1.0 / samplesPerPixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';

}

#endif