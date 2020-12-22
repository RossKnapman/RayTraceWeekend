#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

class material {
    
    public:
        virtual bool scatter(const ray& rIn, const hitRecord& rec, colour& attenuation, ray& scattered) const = 0;

};

class lambertian : public material {

    public:

        colour albedo;

        lambertian(const colour& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hitRecord& rec, colour& attenuation, ray& scattered
        ) const override {

            auto scatterDirection = rec.normal + randomUnitVector();

            // Catch degenerate scatter direction
            if (scatterDirection.nearZero())
                scatterDirection = rec.normal;

            scattered = ray(rec.p, scatterDirection);
            attenuation = albedo;
            return true;
        }
};

class metal : public material {

    public:

        colour albedo;
        double fuzz;

        metal(const colour& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray& r_in, const hitRecord& rec, colour& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unitVector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * randomInUnitSphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

};

class dielectric : public material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const ray& r_in, const hitRecord& rec, colour& attenuation, ray& scattered
        ) const override {
            attenuation = colour(1.0, 1.0, 1.0);
            double refraction_ratio = rec.frontFace ? (1.0/ir) : ir;

            vec3 unit_direction = unitVector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;
            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > randomDouble())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, direction);
            return true;
        }

    public:
        double ir; // Index of Refraction

    private:
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};

#endif