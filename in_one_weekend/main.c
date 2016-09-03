#define _XOPEN_SOURCE /* drand48 */
#include <stdbool.h>

#include "vec3.c"

typedef struct
{
        float T;
        vec3 Point;
        vec3 Normal;
} hit_record;

#include "ray3.c"
#include "sphere.c"
#include "renderable_list.c"
#include "camera.c"

#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <float.h>

vec3
ComputeColor(ray3 Ray, renderable_list *Renderables)
{
        hit_record Record;
        if(RenderableListHit(Renderables, Ray, 0.0, FLT_MAX, &Record))
        {
                vec3 Temp = { 1, 1, 1 };
                vec3 Temp2 = Vec3Add(Record.Normal, Temp);
                vec3 Result = Vec3ScalarMultiply(Temp2, 0.5);

                return(Result);

        }
        else
        {
                vec3 UnitDirection = Vec3Unit(Ray.Direction);
                float T = 0.5 * (UnitDirection.Y + 1.0);

                vec3 A = { 1.0, 1.0, 1.0 };
                A = Vec3ScalarMultiply(A, (1.0 - T));

                vec3 B = { 0.5, 0.7, 1.0 };
                B = Vec3ScalarMultiply(B, T);

                vec3 Result = Vec3Add(A, B);

                return(Result);
        }
}

int
main(int ArgCount, char **Arguments)
{
        int NumCols = 200;
        int NumRows = 100;
        int NumSamples = 100;

        printf("P3\n%i %i\n255\n", NumCols, NumRows);

        renderable_list *Renderables;
        Renderables = RenderableListInit(alloca(RenderableListAllocSize(2)), 2);
        sphere Sphere1 = { 0, 0, -1, 0.5 };
        sphere Sphere2 = { 0, -100.5, -1, 100 };
        RenderableListAdd(Renderables, &Sphere1, SphereHit);
        RenderableListAdd(Renderables, &Sphere2, SphereHit);

        camera Camera;
        CameraInit(&Camera, Vec3Init(0, 0, 0), Vec3Init(-2, -1, -1),
                   Vec3Init(4, 0, 0), Vec3Init(0, 2, 0));

        for(int Y = NumRows-1; Y >= 0; Y--)
        {
                for(int X = 0; X < NumCols; X++)
                {
                        vec3 Color = {0,0,0};
                        for(int S = 0; S < NumSamples; S++)
                        {
                                float U = ((float)X + drand48()) / (float)NumCols;
                                float V = ((float)Y + drand48()) / (float)NumRows;

                                ray3 Ray = CameraGetRay(&Camera, U, V);

                                vec3 ComputedColor = ComputeColor(Ray, Renderables);
                                Color = Vec3Add(Color, ComputedColor);
                        }
                        Color = Vec3ScalarDivide(Color, NumSamples);
                        int RInt = (int)(255.99 * Color.R);
                        int GInt = (int)(255.99 * Color.G);
                        int BInt = (int)(255.99 * Color.B);

                        printf("%i %i %i\n", RInt, GInt, BInt);
                }
        }

        return(EXIT_SUCCESS);
}
