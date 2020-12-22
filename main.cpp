#include "rtweekend.h"

#include "colour.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

colour rayColour(const ray& r, const hittable& world, int depth) {

    // Recursive function that returns the colour of the pixel after the ray has been bounced around

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

    // Background
    vec3 unitDirection = unitVector(r.direction());
    auto t = 0.5*(unitDirection.y() + 1.0);  // t measures height of pixel
    return (1.0-t)*colour(1.0, 1.0, 1.0) + t*colour(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspectRatio = 3.0 / 2.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 20;
    const int maxDepth = 20;

    // World
    hittableList world;

    auto material_ground = make_shared<lambertian>(colour(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(colour(0.7, 0.3, 0.3));
    auto material_left   = make_shared<metal>(colour(0.8, 0.8, 0.8), 0.4);
    auto material_right  = make_shared<metal>(colour(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera
    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto distToFocus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspectRatio, aperture, distToFocus);

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
