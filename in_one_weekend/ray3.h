#ifndef RAY3_H
#define RAY3_H

#include "vec3.h"

typedef struct
{
        vec3 Origin;
        vec3 Direction;
} ray3;

vec3
Ray3Evaluate(ray3 Ray, float Time)
{
        vec3 Scaled = Vec3ScalarMultiply(Ray.Direction, Time);
        vec3 Result = Vec3Add(Ray.Origin, Scaled);
        return(Result);
}

#endif /* RAY3_H */
