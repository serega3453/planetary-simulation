#ifndef ARRAY_H
#define ARRAY_H

#include "types.h"
#include <stdlib.h>

static inline void arrayInit(ObjectArray* objectArray) {
    objectArray->count = 0;
    objectArray->capacity = 100;
    objectArray->objects = malloc(sizeof(Object) * objectArray->capacity);
}

static inline void addObject(ObjectArray* objectArray, Object obj) {
    if (objectArray->count >= objectArray->capacity) {
        objectArray->capacity *= 2;
        objectArray->objects = realloc(objectArray->objects, sizeof(Object) * objectArray->capacity);
    }
    objectArray->objects[objectArray->count++] = obj;
}

static inline void removeObject(ObjectArray* objects, int index) {
    for (int i = index; i < objects->count - 1; i++) {
        objects->objects[i] = objects->objects[i + 1];
    }
    objects->count--;
}

#endif