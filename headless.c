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

int main() {
    ObjectArray objects;
    arrayInit(&objects);
    for (int i = 0; i < 500; i++) {
    Object obj = {
        .mass = randFloat(1.0f, 100.0f),
        .position = randomVec3(-100.0f, 100.0f),
        .velocity = randomVec3(-1.0f, 1.0f)
    };
    addObject(&objects, obj);
}

    int frames = 1000;
    clock_t start = clock();
    
    for (int i = 0; i < frames; i++) {
        simulationStep(&objects, 0.016f);
    }
    
    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("1000 steps in %.3f seconds = %.1f fps\n", seconds, frames / seconds);
    
    return 0;
}