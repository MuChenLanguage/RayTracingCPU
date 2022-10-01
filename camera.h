#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
public:
    camera() {
        lookat = vec3(0, 0, -1);
        lookfrom = vec3(0, 0, 0);
        vup = vec3(0, 1, 0);
        vfov = 40;
    }

    void Init()
    {
        origin = lookfrom;
        vec3 u, v, w;

        auto theta = degrees_to_radians(vfov);
        auto half_height = tan(theta / 2);
        auto half_width = aspect * half_height;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        lower_left_corner = origin - half_width * u - half_height * v - w;

        horizontal = 2 * half_width * u;
        vertical = 2 * half_height * v;
    }

    ray get_ray(double s, double t) {
        return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
    }

public:
    double aspect = 16 / 9;
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 lookfrom, lookat, vup;
    float vfov; // top to bottom, in degrees
};
#endif