#ifndef RAY3_C
#define RAY3_C

#include "vec3.c"

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

ray3
Ray3Init(vec3 Origin, vec3 Direction)
{
        ray3 Ray;
        Ray.Origin = Origin;
        Ray.Direction = Direction;
        return(Ray);
}

#endif /* RAY3_C */
