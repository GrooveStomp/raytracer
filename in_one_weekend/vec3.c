#ifndef VEC3_C
#define VEC3_C

#include <math.h>

typedef struct
{
        union
        {
                struct
                {
                        float X;
                        float Y;
                        float Z;
                };
                struct
                {
                        float R;
                        float G;
                        float B;
                };
        };
} vec3;

vec3
Vec3Init(float A, float B, float C)
{
        vec3 Result;
        Result.X = A;
        Result.Y = B;
        Result.Z = C;
        return(Result);
}

vec3
Vec3Add(vec3 A, vec3 B)
{
        vec3 Result;
        Result.X = A.X + B.X;
        Result.Y = A.Y + B.Y;
        Result.Z = A.Z + B.Z;
        return(Result);
}

vec3
Vec3Subtract(vec3 Minuend, vec3 Subtrahend)
{
        vec3 Result;
        Result.X = Minuend.X - Subtrahend.X;
        Result.Y = Minuend.Y - Subtrahend.Y;
        Result.Z = Minuend.Z - Subtrahend.Z;
        return(Result);
}

float
Vec3Dot(vec3 A, vec3 B)
{
        vec3 C;
        C.X = A.X * B.X;
        C.Y = A.Y * B.Y;
        C.Z = A.Z * B.Z;
        float Result = C.X + C.Y + C.Z;
        return(Result);
}

vec3
Vec3ScalarMultiply(vec3 Vector, float Scalar)
{
        vec3 Result;
        Result.X = Vector.X * Scalar;
        Result.Y = Vector.Y * Scalar;
        Result.Z = Vector.Z * Scalar;
        return(Result);
}

vec3
Vec3ScalarDivide(vec3 Vector, float Scalar)
{
        vec3 Result;
        Result.X = Vector.X / Scalar;
        Result.Y = Vector.Y / Scalar;
        Result.Z = Vector.Z / Scalar;
        return(Result);
}

float
Vec3Length(vec3 Vector)
{
        float X = Vector.X * Vector.X;
        float Y = Vector.Y * Vector.Y;
        float Z = Vector.Z * Vector.Z;
        float Result = sqrt(X + Y + Z);
        return(Result);
}

vec3
Vec3Unit(vec3 Vector)
{
        vec3 Result = Vec3ScalarDivide(Vector, Vec3Length(Vector));
        return(Result);
}

#endif /* VEC3_C */
