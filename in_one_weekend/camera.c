#ifndef CAMERA_C
#define CAMERA_C

#include "vec3.c"
#include "ray3.c"

typedef struct
{
        vec3 Origin;
        vec3 LowerLeftCorner;
        vec3 Horizontal;
        vec3 Vertical;
} camera;

void
CameraInit(camera *Self, vec3 Origin, vec3 LowerLeftCorner, vec3 Horizontal, vec3 Vertical)
{
        Self->Origin = Origin;
        Self->LowerLeftCorner = LowerLeftCorner;
        Self->Horizontal = Horizontal;
        Self->Vertical = Vertical;
}

ray3
CameraGetRay(camera *Self, float U, float V)
{
        vec3 Horizontal = Vec3ScalarMultiply(Self->Horizontal, U);
        vec3 Vertical = Vec3ScalarMultiply(Self->Vertical, V);
        vec3 Accumulator = Vec3Add(Self->LowerLeftCorner, Horizontal);
        Accumulator = Vec3Add(Accumulator, Vertical);
        Accumulator = Vec3Subtract(Accumulator, Self->Origin);

        ray3 Result;
        Result.Origin = Self->Origin;
        Result.Direction = Accumulator;

        return(Result);
}

#endif /* CAMERA_C */
