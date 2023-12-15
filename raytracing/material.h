#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"

class hitRecord;

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray& rIn, const hitRecord& rec, colour& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const colour& a) : albedo(a) {}

    bool scatter(const ray& rIn, const hitRecord& rec, colour& attenuation, ray& scattered)
        const override {
        auto scatter_direction = rec.normal + randomUnitVector();

        // Catch degenerate scatter direction
        if (scatter_direction.nearZero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    colour albedo;
};

class metal : public material {
public:
    metal(const colour& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool scatter(const ray& rIn, const hitRecord& rec, colour& attenuation, ray& scattered)
        const override {
        vec3 reflected = reflect(unitVector(rIn.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * randomUnitVector());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    colour albedo;
    double fuzz;
};

#endif
