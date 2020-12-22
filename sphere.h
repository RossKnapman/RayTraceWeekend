#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:

        point3 centre;
        double radius;
        shared_ptr<material> matPtr;

        sphere() {}
        sphere(point3 cen, double r, shared_ptr<material> m) : centre(cen), radius(r), matPtr(m) {};

        // Override means that an error will be given if the function does not match with the corresponding function from hittable
        virtual bool hit(const ray& r, double tMin, double tMax, hitRecord& rec) const override;

};

bool sphere::hit(const ray& r, double tMin, double tMax, hitRecord& rec) const {

    vec3 oc = r.origin() - centre;
    auto a = r.direction().lengthSquared();
    auto halfB = dot(oc, r.direction());
    auto c = oc.lengthSquared() - radius*radius;

    auto discriminant = halfB*halfB - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range
    // t is effectively the distance along the ray
    // Ray is only retured if root lies within tMin and tMax
    auto root = (-halfB - sqrtd) / a;
    if (root < tMin || tMax < root) {
        root = (-halfB + sqrtd) / a;
        if (root < tMin || tMax < root)
            return false;
    }

    rec.t = root;
    rec.p = r.rayVector(rec.t);
    vec3 outwardNormal = (rec.p - centre) / radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;

    return true;
}

#endif