#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

#include "colour.h"
#include "hittable.h"

#include <iostream>

class camera {
public:
    double aspectRatio = 16.0 / 9.0;
    int imageWidth = 400;
    int samplesPerPixel = 10; // Count of random samples for each pixel
    int maxDepth = 10; // Maximum number of ray bounces into scene

    void render(const hittable& world) {
        initialise();

        std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

        for (int j = 0; j < imageHeight; ++j) {
            std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
            for (int i = 0; i < imageWidth; ++i) {
                colour pixelColour(0, 0, 0);
                for (int sample = 0; sample < samplesPerPixel; ++sample) {
                    ray r = getRay(i, j);
                    pixelColour += rayColour(r, maxDepth, world);
                }
                writeColor(std::cout, pixelColour, samplesPerPixel);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int imageHeight; // Rendered image height
    point3 centre; // Camera center
    point3 pixel00Loc; // Location of pixel 0, 0
    vec3 pixelDeltaU; // Offset to pixel to the right
    vec3 pixelDeltaV; // Offset to pixel below

    void initialise() {
        imageHeight = static_cast<int>(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;

        centre = point3(0, 0, 0);

        // Determine viewport dimensions.
        auto focalLength = 1.0;
        auto viewportHeight = 2.0;
        auto viewportWidth = viewportHeight * (static_cast<double>(imageWidth) / imageHeight);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewportU = vec3(viewportWidth, 0, 0);
        auto viewportV = vec3(0, -viewportHeight, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixelDeltaU = viewportU / imageWidth;
        pixelDeltaV = viewportV / imageHeight;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
            centre - vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
        pixel00Loc = viewport_upper_left + 0.5 * (pixelDeltaU + pixelDeltaV);
    }

    ray getRay(int i, int j) const {
        // Get a randomly sampled camera ray for the pixel at location i,j.

        auto pixel_center = pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
        auto pixel_sample = pixel_center + pixelSampleSquare();

        auto ray_origin = centre;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 pixelSampleSquare() const {
        // Returns a random point in the square surrounding a pixel at the origin.
        auto px = -0.5 + randomDouble();
        auto py = -0.5 + randomDouble();
        return (px * pixelDeltaU) + (py * pixelDeltaV);
    }

    colour rayColour(const ray& r, int depth, const hittable& world) {
        hitRecord rec;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return colour(0, 0, 0);

        if (world.hit(r, interval(0.001, infinity), rec)) {
            vec3 direction = randomOnHemisphere(rec.normal);
            return 0.5 * rayColour(ray(rec.p, direction), depth - 1, world);
        }

        vec3 unitDirection = unitVector(r.direction());
        auto a = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1.0);
    }
};

#endif
