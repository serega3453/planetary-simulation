#include "types.h"
#include "array.h"

void gravityForce(Object* obj1, Object* obj2, float dt)
{
	float G = 0.2;

	Vec3 diff = substractVec3(obj2->position, obj1->position);

	float r = distanceVec3(obj1->position, obj2->position) + 0.2f;

	float F = G * obj1->mass * obj2->mass / pow(r, 2);

	Vec3 force = scaleVec3(diff, F / r);

	obj1->velocity = addVec3(obj1->velocity, scaleVec3(force, dt / obj1->mass));
	obj2->velocity = substractVec3(obj2->velocity, scaleVec3(force, dt / obj2->mass));
}

void collisionResolver(ObjectArray* objectArray, int i, int j)
{
	if (objectArray->objects[i].mass >= objectArray->objects[j].mass)
	{
		objectArray->objects[i].mass += objectArray->objects[j].mass;
		objectArray->objects[i].radius = powf(objectArray->objects[i].mass, 1.0 / 3.0);
		removeObject(objectArray, j);
	}
	else
	{
		objectArray->objects[j].mass += objectArray->objects[i].mass;
		objectArray->objects[j].radius = powf(objectArray->objects[j].mass, 1.0 / 3.0);
		removeObject(objectArray, i);
	}
}

int collisionDetect(Object obj1, Object obj2)
{
	return distanceVec3(obj1.position, obj2.position) <= (obj1.radius + obj2.radius);
}

void simulationStep(ObjectArray* objectArray, float dt)
{
	for (int i = 0; i < objectArray->count; i++)
	{
		for (int j = i + 1; j < objectArray->count; j++)
		{
			gravityForce(&objectArray->objects[i], &objectArray->objects[j], dt);

			if (collisionDetect(objectArray->objects[i], objectArray->objects[j]))
			{
				collisionResolver(objectArray, i, j);
			}
		}
	}
	for (int i = 0; i < objectArray->count; i++)
	{
		objectArray->objects[i].position = addVec3(objectArray->objects[i].position, scaleVec3(objectArray->objects[i].velocity, dt));
	}
}