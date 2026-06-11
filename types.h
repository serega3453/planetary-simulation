#ifndef TYPES_H
#define TYPES_H

typedef struct
{
    double x, y, z;
} Vec3;

typedef struct
{
    double x, y;
} Vec2;

typedef struct
{
    double mass;
    Vec3 position;
    Vec3 velocity;
    Model model;
} Object;

typedef struct
{
    Object* objects;
    int count;
    int capacity;
} ObjectArray;

#endif