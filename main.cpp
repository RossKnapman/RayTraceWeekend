#include "rtweekend.h"

#include "colour.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

colour rayColour(const ray& r, const hittable& world, int depth) {
    hitRecord rec;

    // If we've exceeded the ray bounce limit, no more light is gathered
    if (depth <= 0) {
        return colour(0, 0, 0);
    }

    // The 0.001 stops self-shadows if t is close to zero, but not exactly zero due to numerical error ("shadow acne")
    if (world.hit(r, 0.001, infinity, rec)) {

        ray scattered;
        colour attenuation;
        if (rec.matPtr->scatter(r, rec, attenuation, scattered))
            return attenuation * rayColour(scattered, world, depth-1);
        return colour(0,0,0);
        
    }

    vec3 unitDirection = unitVector(r.direction());
    auto t = 0.5*(unitDirection.y() + 1.0);
    return (1.0-t)*colour(1.0, 1.0, 1.0) + t*colour(0.5, 0.7, 1.0);
}

hittableList random_scene() {
    hittableList world;

    auto ground_material = make_shared<lambertian>(colour(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomDouble();
            point3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = colour::random() * colour::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = colour::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(colour(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main() {

    // Image

    const auto aspect_ratio = 3.0 / 2.0;
    const int imageWidth = 1200;
    const int imageHeight = static_cast<int>(imageWidth / aspect_ratio);
    const int samplesPerPixel = 100;
    const int maxDepth = 20;

    // World
    auto world = random_scene();

    // Camera
    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight-1; j >= 0; --j) {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = 0; i < imageWidth; ++i) {
                colour pixelColour(0, 0, 0);

                // Loop and take samples for anti-aliasing
                for (int s = 0; s < samplesPerPixel; ++s) {
                    auto u = (i + randomDouble()) / (imageWidth-1);
                    auto v = (j + randomDouble()) / (imageHeight-1);
                    ray r = cam.getRay(u, v);
                    pixelColour += rayColour(r, world, maxDepth);
                }
                writeColour(std::cout, pixelColour, samplesPerPixel);
            }
        }

    std::cerr << "\nDone.\n";
}
