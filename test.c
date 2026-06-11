#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "raymath.h"
#include "types.h"
#include "utils.h"
#include "simulation.c"
#include "rlgl.h"
#include "array.h"

Vector3 vecToVector(Vec3 vec)
{
    return (Vector3){vec.x, vec.y, vec.z};
}

void drawObject(ObjectArray* objectArray, int index)
{
    DrawModel(objectArray->objects[index].model, vecToVector(objectArray->objects[index].position), objectArray->objects[index].mass, ORANGE);
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
    DrawFPS(10, 10);
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
    double distanceControls = 0.0;
    double speed = 10;
    double scrollSpeed = 20;

    if (IsKeyDown(KEY_W)) controls.y += speed;
    if (IsKeyDown(KEY_S)) controls.y -= speed;
    if (IsKeyDown(KEY_A)) controls.x -= speed;
    if (IsKeyDown(KEY_D)) controls.x += speed;

    if (IsKeyDown(KEY_SPACE)) controls.z += speed;
    if (IsKeyDown(KEY_LEFT_CONTROL)) controls.z -= speed;

    if (IsKeyDown(KEY_Q)) distanceControls -= scrollSpeed;
    if (IsKeyDown(KEY_E)) distanceControls += scrollSpeed;

    Vec3 dirMove = (Vec3){
        (dirNorm.x * controls.y) + (rightNorm.x * controls.x) + (dirNorm.x * distanceControls),
        (dirNorm.y * controls.y) + controls.z + (dirNorm.y * distanceControls),
        (dirNorm.z * controls.y) + (rightNorm.z * controls.x) + (dirNorm.z * distanceControls)
    };

    cam->position.x += dirMove.x;
    cam->position.y += dirMove.y;
    cam->position.z += dirMove.z;

    //if (Vector3Distance(cam->position, cam->target) > 100.0f) cam->position = Vector3Add(cam->position, (Vector3){dirNorm.x*2, dirNorm.y*2, dirNorm.z*2});
}

void camTargetSet(Camera* cam, Object obj)
{
    cam->target = (Vector3){obj.position.x, obj.position.y, obj.position.z};
}

void camTargetCycling(Camera* cam, ObjectArray* objectArray, int* i)
{
    if (IsKeyPressed(KEY_TAB)) {
        *i = (*i + 1) % objectArray->count;
        // reset position relative to new target
        cam->position = (Vector3){
            objectArray->objects[*i].position.x + 10,
            objectArray->objects[*i].position.y,
            objectArray->objects[*i].position.z
        };
    }
    
    // store offset before moving target
    Vector3 offset = {
        cam->position.x - cam->target.x,
        cam->position.y - cam->target.y,
        cam->position.z - cam->target.z
    };
    
    // move target to object
    camTargetSet(cam, objectArray->objects[*i]);
    
    // apply same offset to new target position
    cam->position.x = cam->target.x + offset.x;
    cam->position.y = cam->target.y + offset.y;
    cam->position.z = cam->target.z + offset.z;
}

int main()
{
    srand(time(NULL));

    float timeScale = 10.0;

    ObjectArray objectArray;
    arrayInit(&objectArray);

    int cameraTargetIterator = 0;

    InitWindow(640, 480, "test");
    Camera3D camera = {
        .position = {0, 10, 10},
        .target = {0, 0, 0},
        .up = {0, 1, 0},
        .fovy = 60,
        .projection = CAMERA_PERSPECTIVE
    };

    rlSetClipPlanes(0.01, 100000.0);

    SetTargetFPS(60);

    Model sphere = LoadModelFromMesh(GenMeshSphere(1.0f, 16, 16));
    Shader shader = shaderInit();

    sphere.materials[0].shader = shader;

    Light light = CreateLight(LIGHT_POINT, (Vector3){0,5,0}, (Vector3){0, 0, 0}, WHITE, shader);

    addObject(&objectArray, (Object){100.0, (Vec3){0.0, 0.0, 0.0}, (Vec3){0.0, 0.0, 0.0}, sphere});

    for (int i = 0; i < 50; i++)
    {
        addObject(&objectArray, (Object){randFloat(1.0, 10.0), randomVec3(-500.0, 500.0), randomVec3(-0.5, 0.5), sphere});
    }

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        simulationStep(&objectArray, dt * timeScale);
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);
        moveCamera(&camera);
        camTargetCycling(&camera, &objectArray, &cameraTargetIterator);
        UpdateLightValues(shader, light);
        draw3D(&objectArray);
        EndMode3D();
        draw2D();
        DrawText(TextFormat("Time scale: %.00f", timeScale), 10, 30, 20, WHITE); //temp
        DrawText(TextFormat("Object: %d", cameraTargetIterator), 10, 60, 20, WHITE); //temp
        EndDrawing();

        if (IsKeyDown(KEY_EQUAL)) timeScale *= 1.01f; //temp
        if (IsKeyDown(KEY_MINUS)) timeScale *= 0.99f; //temp
    }
    CloseWindow();
    return 0;
}