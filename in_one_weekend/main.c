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
        int X = 200;
        int Y = 100;

        printf("P3\n%i %i\n255\n", X, Y);

        vec3 LowerLeftCorner = { -2.0, -1.0, -1.0 };
        vec3 Horizontal = { 4.0, 0.0, 0.0 };
        vec3 Vertical = { 0.0, 2.0, 0.0 };
        vec3 Origin = { 0.0, 0.0, 0.0 };

        renderable_list *Renderables;
        Renderables = RenderableListInit(alloca(RenderableListAllocSize(2)), 2);
        sphere Sphere1 = { 0, 0, -1, 0.5 };
        sphere Sphere2 = { 0, -100.5, -1, 100 };
        RenderableListAdd(Renderables, &Sphere1, SphereHit);
        RenderableListAdd(Renderables, &Sphere2, SphereHit);

        for(int J = Y-1; J >= 0; J--)
        {
                for(int I = 0; I < X; I++)
                {
                        float U = (float)I / (float)X;
                        float V = (float)J / (float)Y;

                        vec3 UHorizontal = Vec3ScalarMultiply(Horizontal, U);
                        vec3 VVertical = Vec3ScalarMultiply(Vertical, V);

                        vec3 Sum;
                        Sum = Vec3Add(LowerLeftCorner, UHorizontal);
                        Sum = Vec3Add(Sum, VVertical);
                        ray3 Ray = { Origin, Sum };

                        vec3 Color = ComputeColor(Ray, Renderables);
                        int RInt = (int)(255.99 * Color.R);
                        int GInt = (int)(255.99 * Color.G);
                        int BInt = (int)(255.99 * Color.B);

                        printf("%i %i %i\n", RInt, GInt, BInt);
                }
        }

        return(EXIT_SUCCESS);
}
