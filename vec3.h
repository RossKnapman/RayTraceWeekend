#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
    public:

        double e[3];

        vec3() : e{0,0,0} {}  // Initialization list
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        // Const makes it a compiler error for this function to change a member variable of the class
        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }  // Define inverse of the vector
        double operator[](int i) const { return e[i]; }  // Define operator to get element of vector
        double& operator[] (int i) { return e[i]; }  // Returns a reference to a double whereas above only returns value, so allows you to assign values given an index

        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(const double t) {
            return *this *= 1/t;
        }

        double length() const {
            return sqrt(lengthSquared());
        }

        double lengthSquared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        inline static vec3 random() {
            return vec3(randomDouble(), randomDouble(), randomDouble());
        }

        inline static vec3 random(double min, double max) {
            return vec3(randomDouble(min,max), randomDouble(min,max), randomDouble(min,max));
        }

        bool nearZero() const {
            // Return true if vector is close to zero in both directions (which would result in zero scatter)
            const auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        }

};

// Type aliases for vec3
// This allows us to declare variables using these instead of vec3
using point3 = vec3;  // 3D point
using colour = vec3;  // RGB colour

// vec3 utility functions (functions that support the operations of other functions within the class)
// Inline functions are expanded in the line in which they are called, rather than though actually defining the function

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unitVector(vec3 v) {
    return v / v.length();
}

inline vec3 randomInUnitSphere() {
    while (true) {
        auto p = vec3::random(-1,1);
        if (p.lengthSquared() >= 1) continue;
        return p;
    }
}

inline vec3 randomUnitVector() {
    // Get random point *on* the unit sphere, not *in* (centred at unit vector from surface normal)
    return unitVector(randomInUnitSphere());
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.lengthSquared())) * n;
    return r_out_perp + r_out_parallel;
}

inline vec3 randomInUnitDisk() {
    while (true) {
        auto p = vec3(randomDouble(-1,1), randomDouble(-1,1), 0);
        if (p.lengthSquared() >= 1) continue;
        return p;
    }
}

#endif