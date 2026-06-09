#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "raymath.h"

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
    DrawModel(objectArray->objects[index].model, vecToVector(objectArray->objects[index].position), 1.0, ORANGE);
}

void draw3D(ObjectArray* objectArray)
{
    for (int i = 0; i < objectArray->count; i++)
    {
        drawObject(objectArray, i);
    }
    DrawGrid(10, 1.0f);
}

Shader shaderInit()
{
    Shader shader = LoadShader("lighting.vs", "lighting.fs");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    float ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
    return shader;
}

void draw2D()
{


}

void moveCamera(Camera* cam)
{
    double dx = cam->target.x - cam->position.x;
    double dy = cam->target.y - cam->position.y;
    double dz = cam->target.z - cam->position.z;

    double dirMag = sqrt(dx * dx + dy * dy + dz * dz);

    Vec3 dirNorm = (Vec3){dx / dirMag, dy / dirMag, dz / dirMag};

    Vec3 rightNorm = (Vec3){-dirNorm.z, 0.0, dirNorm.x};

    Vec3 controls = (Vec3){0.0, 0.0, 0.0};
    double speed = 0.05;

    if (IsKeyDown(KEY_W)) controls.y += speed;
    if (IsKeyDown(KEY_S)) controls.y -= speed;
    if (IsKeyDown(KEY_A)) controls.x -= speed;
    if (IsKeyDown(KEY_D)) controls.x += speed;

    if (IsKeyDown(KEY_SPACE)) controls.z += speed;
    if (IsKeyDown(KEY_LEFT_CONTROL)) controls.z -= speed;

    Vec3 dirMove = (Vec3){
        (dirNorm.x * controls.y) + (rightNorm.x * controls.x),
        (dirNorm.y * controls.y) + controls.z,
        (dirNorm.z * controls.y) + (rightNorm.z * controls.x)
    };

    cam->position.x += dirMove.x;
    cam->position.y += dirMove.y;
    cam->position.z += dirMove.z;
}

int main()
{
    srand(time(NULL));

    ObjectArray objectArray;
    arrayInit(&objectArray);

    InitWindow(640, 480, "test");
    Camera3D camera = {
        .position = {0, 10, 10},
        .target = {0, 0, 0},
        .up = {0, 1, 0},
        .fovy = 60,
        .projection = CAMERA_PERSPECTIVE
    };

   SetTargetFPS(60);

    Model sphere = LoadModelFromMesh(GenMeshSphere(1.0f, 16, 16));
    Shader shader = shaderInit();

    sphere.materials[0].shader = shader;

    Light light = CreateLight(LIGHT_POINT, (Vector3){0,5,0}, (Vector3){0, 0, 0}, WHITE, shader);

    for (int i = 0; i < 10; i++)
    {
        addObject(&objectArray, (Object){1.0, randomVec3(-5.0, 5.0), (Vec3){0.0, 0.0, 0.0}, sphere});
    }

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);
        moveCamera(&camera);
        UpdateLightValues(shader, light);
        draw3D(&objectArray);
        EndMode3D();
        draw2D();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}