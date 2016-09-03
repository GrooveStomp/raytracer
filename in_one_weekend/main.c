#define _XOPEN_SOURCE /* drand48 */
#include <stdbool.h>
#include <math.h>

/******************************************************************************
 * vec3 Interface
 ******************************************************************************/
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

vec3
Vec3Multiply(vec3 A, vec3 B)
{
        vec3 Result;
        Result.X = A.X * B.X;
        Result.Y = A.Y * B.Y;
        Result.Z = A.Z * B.Z;
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

vec3
Vec3Reflect(vec3 Vector, vec3 Normal)
{
        float Dot = Vec3Dot(Vector, Normal);
        vec3 Result = Vec3ScalarMultiply(Normal, Dot);
        Result = Vec3ScalarMultiply(Result, 2);
        Result = Vec3Subtract(Vector, Result);

        return(Result);
}

/******************************************************************************
 * ray3 Interface
 ******************************************************************************/
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

/******************************************************************************
 * Hittables and hit_record
 ******************************************************************************/
typedef struct hit_record
{
        float T;
        vec3 Point;
        vec3 Normal;
        void *Material; /* lambertian, metal */
        bool (*ScatterFn)(void *Material, ray3 Ray, struct hit_record *Record, vec3 *Attenuation, ray3 *Scattered);
} hit_record;

/******************************************************************************
 * Forward-declared functions and types (ie., global functions used everywhere.)
 ******************************************************************************/
vec3
RandomPointInUnitSphere(void);

typedef bool (*hit_fn)(void *Renderable, ray3 Ray, float Min, float Max, hit_record *Record);
typedef bool (*scatter_fn)(void *Material, ray3 Ray, hit_record *Record, vec3 *Attenuation, ray3 *Scattered);

/******************************************************************************
 * Sphere (Hittable) interface
 ******************************************************************************/
typedef struct
{
        vec3 Center;
        float Radius;
        void *Material; /* lambertian, metal */
        scatter_fn ScatterFn;
} sphere;

sphere
SphereInit(vec3 Center, float Radius, void *Material, scatter_fn ScatterFn)
{
        sphere Result;
        Result.Center = Center;
        Result.Radius = Radius;
        Result.Material = Material;
        Result.ScatterFn = ScatterFn;
        return(Result);
}

bool
SphereHit(void *Renderable, ray3 Ray, float Min, float Max, hit_record *Record)
{
        sphere *Self = (sphere *)Renderable;
        vec3 ToCircle = Vec3Subtract(Ray.Origin, Self->Center);
        float A = Vec3Dot(Ray.Direction, Ray.Direction);
        float B = Vec3Dot(ToCircle, Ray.Direction);
        float C = Vec3Dot(ToCircle, ToCircle) - (Self->Radius * Self->Radius);
        float Discriminant = B * B - A * C;

        if(Discriminant > 0)
        {
                float Temp = (-B - sqrt(Discriminant)) / A;
                if(Temp < Max && Temp > Min)
                {
                        Record->T = Temp;
                        Record->Point = Ray3Evaluate(Ray, Temp);
                        vec3 Temp2 = Vec3Subtract(Record->Point, Self->Center);
                        Record->Normal = Vec3ScalarDivide(Temp2, Self->Radius);
                        Record->Material = Self->Material;
                        Record->ScatterFn = Self->ScatterFn;

                        return(true);
                }
                Temp = (-B + sqrt(Discriminant)) / A;
                if(Temp < Max && Temp > Min)
                {
                        Record->T = Temp;
                        Record->Point = Ray3Evaluate(Ray, Temp);
                        vec3 Temp2 = Vec3Subtract(Record->Point, Self->Center);
                        Record->Normal = Vec3ScalarDivide(Temp2, Self->Radius);
                        Record->Material = Self->Material;
                        Record->ScatterFn = Self->ScatterFn;

                        return(true);
                }
        }

        return(false);
}

/******************************************************************************
 * renderable_list Interface
 ******************************************************************************/
#include <string.h> /* memset */

typedef struct
{
        void **Renderables;
        hit_fn *HitFunctions;
        unsigned int Count;
        unsigned int Capacity;
        unsigned int NumBytes;
} renderable_list;

unsigned int
RenderableListAllocSize(unsigned int Count)
{
        unsigned int ListSize = sizeof(renderable_list);
        unsigned int ObjPtrSize = sizeof(void *) * Count;
        unsigned int HitFnPtrSize = sizeof(hit_fn) * Count;
        unsigned int Result = ListSize + ObjPtrSize + HitFnPtrSize;
        return(Result);
}

renderable_list * /* Memory must be RenderableListAllocSize large. */
RenderableListInit(void *Memory, unsigned int Count)
{
        renderable_list *Result = (renderable_list *)Memory;
        Result->Count = 0;
        Result->Capacity = Count;
        Result->NumBytes = RenderableListAllocSize(Count);

        Result->Renderables = (void **)((char *)Memory + sizeof(renderable_list));
        memset((char *)Result->Renderables, 0, sizeof(void *) * Count);

        Result->HitFunctions = (hit_fn *)(
                (char *)Memory +
                sizeof(renderable_list) +
                (sizeof(void *) * Count)
        );
        memset((char *)Result->HitFunctions, 0, sizeof(hit_fn) * Count);

        return(Result);
}

bool
RenderableListHit(renderable_list *Self, ray3 Ray, float Min, float Max, hit_record *Record)
{
        hit_record TempRecord;
        bool HitAnything = false;
        double ClosestSoFar = Max;

        for(int I = 0; I < Self->Count; I++)
        {
                void *Renderable = Self->Renderables[I];
                hit_fn HitFn = Self->HitFunctions[I];
                if(HitFn(Renderable, Ray, Min, ClosestSoFar, &TempRecord))
                {
                        HitAnything = true;
                        ClosestSoFar = TempRecord.T;
                        *Record = TempRecord;
                }
        }

        return(HitAnything);
}

bool
RenderableListAdd(renderable_list *Self, void *Renderable, hit_fn HitFn)
{
        if(Self->Count >= Self->Capacity) return(false);

        unsigned int Index = Self->Count;
        Self->Renderables[Index] = Renderable;
        Self->HitFunctions[Index] = HitFn;
        Self->Count++;

        return(true);
}

/******************************************************************************
 * camera Interface
 ******************************************************************************/
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

/******************************************************************************
 * lambertian (Material) Interface
 ******************************************************************************/
typedef struct
{
        vec3 Albedo;
} lambertian;

lambertian
LambertianInit(vec3 Albedo)
{
        lambertian Result;
        Result.Albedo = Albedo;
        return(Result);
}

bool
LambertianScatter(void *Material, ray3 Ray, hit_record *Record, vec3 *Attenuation, ray3 *Scattered)
{
        lambertian *Self = (lambertian *)Material;
        vec3 Target = Vec3Add(Record->Point, Record->Normal);
        Target = Vec3Add(Target, RandomPointInUnitSphere());
        *Scattered = Ray3Init(Record->Point, Vec3Subtract(Target, Record->Point));
        *Attenuation = Self->Albedo;
        return(true);
}

/******************************************************************************
 * metal (Material) Interface
 ******************************************************************************/
typedef struct
{
        vec3 Albedo;
        float Fuzziness;
} metal;

metal
MetalInit(vec3 Albedo, float Fuzziness)
{
        metal Result;
        Result.Albedo = Albedo;

        if(Fuzziness < 1)
                Result.Fuzziness = Fuzziness;
        else
                Result.Fuzziness = 1;

        return(Result);
}

bool
MetalScatter(void *Material, ray3 Ray, hit_record *Record, vec3 *Attenuation, ray3 *Scattered)
{
        metal *Self = (metal *)Material;
        vec3 Reflected = Vec3Reflect(Vec3Unit(Ray.Direction), Record->Normal);

        vec3 Fuzzed = Vec3ScalarMultiply(RandomPointInUnitSphere(), Self->Fuzziness);
        Reflected = Vec3Add(Reflected, Fuzzed);
        *Scattered = Ray3Init(Record->Point, Reflected);
        *Attenuation = Self->Albedo;

        float Dot = Vec3Dot(Scattered->Direction, Record->Normal);
        bool Result = Dot > 0;
        return(Result);
}

/******************************************************************************
 * main()
 ******************************************************************************/

/* The following #includes are only required for main() */
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <float.h>

vec3
RandomPointInUnitSphere(void)
{
        vec3 Point;

        float SquaredLength = 0;
        do {
                vec3 Temp = Vec3Init(drand48(), drand48(), drand48());
                Temp = Vec3ScalarMultiply(Temp, 2.0);
                Temp = Vec3Subtract(Temp, Vec3Init(1,1,1));
                Point = Temp;

                float Length = Vec3Length(Point);
                SquaredLength = Length * Length;

        } while(SquaredLength >= 1.0);

        return(Point);
}

vec3
ComputeColor(ray3 Ray, renderable_list *Renderables, unsigned int Depth)
{
        hit_record Record;
        if(RenderableListHit(Renderables, Ray, 0.001, FLT_MAX, &Record))
        {
                ray3 Scattered;
                vec3 Attenuation;

                if(Depth < 50 && Record.ScatterFn(Record.Material, Ray, &Record, &Attenuation, &Scattered))
                {
                        vec3 Result = ComputeColor(Scattered, Renderables, Depth + 1);
                        Result = Vec3Multiply(Result, Attenuation);

                        return(Result);
                }
                else
                {
                        vec3 Result = {0,0,0};
                        return(Result);
                }
        }
        else
        {
                /* Compute background gradient. */
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

        unsigned int NumRenderables = 4;
        renderable_list *Renderables;
        Renderables = RenderableListInit(alloca(RenderableListAllocSize(NumRenderables)), NumRenderables);

        lambertian Material1 = LambertianInit(Vec3Init(0.8, 0.3, 0.3));
        sphere Sphere1 = SphereInit(Vec3Init(0,0,-1), 0.5,
                                    &Material1, LambertianScatter);
        RenderableListAdd(Renderables, &Sphere1, SphereHit);

        lambertian Material2 = LambertianInit(Vec3Init(0.8, 0.8, 0.0));
        sphere Sphere2 = SphereInit(Vec3Init(0,-100.5,-1), 100,
                                    &Material2, LambertianScatter);
        RenderableListAdd(Renderables, &Sphere2, SphereHit);

        metal Material3 = MetalInit(Vec3Init(0.8, 0.6, 0.2), 0.3);
        sphere Sphere3 = SphereInit(Vec3Init(1,0,-1), 0.5,
                                    &Material3, MetalScatter);
        RenderableListAdd(Renderables, &Sphere3, SphereHit);

        metal Material4 = MetalInit(Vec3Init(0.8, 0.8, 0.8), 1.0);
        sphere Sphere4 = SphereInit(Vec3Init(-1,0,-1), 0.5,
                                    &Material4, MetalScatter);
        RenderableListAdd(Renderables, &Sphere4, SphereHit);

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

                                vec3 ComputedColor = ComputeColor(Ray, Renderables, 0);
                                Color = Vec3Add(Color, ComputedColor);
                        }
                        Color = Vec3ScalarDivide(Color, NumSamples);
                        /* Perform gamma correction with gamma == 2 */
                        Color = Vec3Init(sqrt(Color.R), sqrt(Color.G), sqrt(Color.B));
                        int RInt = (int)(255.99 * Color.R);
                        int GInt = (int)(255.99 * Color.G);
                        int BInt = (int)(255.99 * Color.B);

                        printf("%i %i %i\n", RInt, GInt, BInt);
                }
        }

        return(EXIT_SUCCESS);
}
