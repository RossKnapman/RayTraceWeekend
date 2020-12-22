#ifndef HTTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

// shared_ptr<type> has a reference count, which is incremented when its value is assigned to another shared pointer
// If the shared pointer goes out of scope, the reference count is decremented
// When the count goes to zero, the object is deleted

class hittableList : public hittable {

    public:
        std::vector<shared_ptr<hittable>> objects;  // Vector of pointers to objects

        hittableList() {}
        hittableList(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(const ray& r, double tmin, double tmax, hitRecord& rec) const override;

};

bool hittableList::hit(const ray& r, double tmin, double tmax, hitRecord& rec) const {

    hitRecord tempRec;
    bool hitAnything = false;
    auto closestSoFar = tmax;

    for (const auto& object : objects) {
        if (object->hit(r, tmin, closestSoFar, tempRec)) {
            hitAnything = true;
            closestSoFar= tempRec.t;
            rec = tempRec;
        }
    }
    
    return hitAnything;
}

#endif