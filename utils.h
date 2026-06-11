#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <stdlib.h>
#include <math.h>

static inline float randFloat(float min, float max)
{
    return min + (((float)rand() / (float)RAND_MAX) * (max - min));
}

static inline Vec3 randomVec3(float min, float max)
{
    return (Vec3){randFloat(min, max), randFloat(min, max), randFloat(min, max)};
}

static inline float distanceVec3(Vec3 a, Vec3 b)
{
    return (sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2)));
}

static inline float magnitudeVec3(Vec3 a)
{
    return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
}

static inline Vec3 substractVec3(Vec3 a, Vec3 b)
{
    return (Vec3){a.x-b.x, a.y-b.y, a.z-b.z};
}

static inline Vec3 addVec3(Vec3 a, Vec3 b)
{
    return (Vec3){a.x+b.x, a.y+b.y, a.z+b.z};
}

static inline Vec3 scaleVec3(Vec3 a, float b)
{
    return (Vec3){a.x * b, a.y * b, a.z * b};
}
#endif