#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

#include <memory>
using std::shared_ptr;

class material;  // Alert the compiler that the pointer is to a class

struct hitRecord {

    // Basically to avoid a load of arguments
    point3 p;
    vec3 normal;
    shared_ptr<material> matPtr;
    double t;
    bool frontFace;

    inline void setFaceNormal(const ray& r, const vec3& outwardNormal) {
        // Determine which side of the surface the ray is coming from (important for objects with e.g. different materials on different sides)
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal :-outwardNormal;
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hitRecord& rec) const = 0;
};

#endif