#include <stdio.h>
#include <stdlib.h>

#include "vec3.h"
#include "ray3.h"

vec3 ComputeColor(ray3 *Ray)
{
        vec3 UnitDirection = Vec3Unit(RayDirection(Ray));
        float T = 0.5 * (UnitDirection.Y + 1.0);

        vec3 A = { 1.0, 1.0, 1.0 };;
//        Vec3Init(&A, 1.0, 1.0, 1.0);
        A = Vec3ScalarMultiply(A, (1.0 - T));

        vec3 B = { 0.5, 0.7, 1.0 };;
//        Vec3Init(&B, 0.5, 0.7, 1.0);
        B = Vec3ScalarMultiply(B, T);

        vec3 Result = Vec3VectorAdd(A, B);
        return(Result);
}

int
main(int ArgCount, char **Arguments)
{
        int X = 200;
        int Y = 100;

        printf("P3\n%i %i\n255\n", X, Y);

        vec3 LowerLeftCorner = { -2.0, -1.0, -1.0 }; //Vec3Init((vec3 *)alloca(sizeof(vec3), -2.0, -1.0, -1.0));
        vec3 Horizontal = { 4.0, 0.0, 0.0 }; //Vec3Init((vec3 *)alloca(sizeof(vec3), 4.0, 0.0, 0.0));
        vec3 Vertical = { 0.0, 2.0, 0.0 }; //Vec3Init((vec3 *)alloca(sizeof(vec3), 0.0, 2.0, 0.0));
        vec3 Origin = { 0.0, 0.0, 0.0 }; //Vec3Init((vec3 *)alloca(sizeof(vec3)), 0.0, 0.0, 0.0);

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

                        vec3 Color = ComputeColor(Sum);

                        int RInt = (int)(255.99 * Color.R);
                        int GInt = (int)(255.99 * Color.G);
                        int BInt = (int)(255.99 * Color.B);

                        printf("%i %i %i\n", RInt, GInt, BInt);
                }
        }

        return(EXIT_SUCCESS);
}
