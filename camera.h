#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {

    public:

        camera(point3 lookFrom, point3 lookAt, vec3 vUp, double vfov, double aspectRatio, double aperture, double focusDist) {

            auto theta = deg2Rad(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspectRatio * viewport_height;

            auto w = unitVector(lookFrom - lookAt);
            auto u = unitVector(cross(vUp, w));
            auto v = cross(w, u);

            origin = lookFrom;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lowerLeftCorner = origin - horizontal/2 - vertical/2 - w;
        }

        ray getRay(double s, double t) const {
            // Thin lens approximation, randomness induces blurriness associated with a larger aperture
            vec3 rd = lensRadius * randomInUnitDisk();
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(
                origin + offset,
                lowerLeftCorner + s*horizontal + t*vertical - origin - offset
            );
        }

    private:

        point3 origin;
        point3 lowerLeftCorner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lensRadius;

};

#endif