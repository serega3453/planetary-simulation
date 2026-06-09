#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

typedef struct
{
    double x, y, z;
} Vec3;

typedef struct
{
    double mass;
    Vec3 position;
    Vec3 velocity;
} Object;

typedef struct
{
    Object* objects;
    int count;
    int capacity;
} ObjectArray;

float randFloat(float min, float max)
{
    return min + (((float)rand() / (float)RAND_MAX) * (max - min));
}

void arrayInit(ObjectArray* objectArray)
{
    objectArray->count = 0;
    objectArray->capacity = 100;
    objectArray->objects = malloc(sizeof(Object) * objectArray->capacity);
}

void addObject(ObjectArray* objectArray, Object obj)
{
    if (objectArray->count >= objectArray->capacity)
    {
        objectArray->capacity *= 2;
        objectArray->objects = realloc(objectArray->objects, sizeof(Object) * objectArray->capacity);
    }
    objectArray->objects[objectArray->count++] = obj;
}

Vector3 vecToVector(Vec3 vec)
{
    return (Vector3){vec.x, vec.y, vec.z};
}

Vec3 randomVec3(float min, float max)
{
    return (Vec3){randFloat(min, max), randFloat(min, max), randFloat(min, max)};
}

void drawObject(ObjectArray* objectArray, int index)
{
    DrawSphere(vecToVector(objectArray->objects[index].position), 1.0, ORANGE);
}

void draw3D(ObjectArray* objectArray)
{
    DrawSphere((Vector3){0, 0, 0}, 0.3f, ORANGE);
    for (int i = 0; i < objectArray->count; i++)
    {
        drawObject(objectArray, i);
    }
    DrawGrid(10, 1.0f);
}

void draw2D()
{


}

int main()
{
    srand(time(NULL));

    ObjectArray objectArray;
    arrayInit(&objectArray);

    for (int i = 0; i < 10; i++)
    {
        addObject(&objectArray, (Object){1.0, randomVec3(-5.0, 5.0), (Vec3){0.0, 0.0, 0.0}});
    }

    InitWindow(640, 480, "test");
    Camera3D camera = {
        .position = {0, 10, 20},
        .target = {0, 0, 0},
        .up = {0, 1, 0},
        .fovy = 45,
        .projection = CAMERA_PERSPECTIVE
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);
        draw3D(&objectArray);
        EndMode3D();
        draw2D();
        EndDrawing();
    }
    CloseWindow();
    //return 0;
}