#ifndef SPHERE_C
#define SPHERE_C

#include "vec3.c"
#include "ray3.c"

typedef struct
{
        vec3 Center;
        float Radius;
} sphere;

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

                        return(true);
                }
                Temp = (-B + sqrt(Discriminant)) / A;
                if(Temp < Max && Temp > Min)
                {
                        Record->T = Temp;
                        Record->Point = Ray3Evaluate(Ray, Temp);
                        vec3 Temp2 = Vec3Subtract(Record->Point, Self->Center);
                        Record->Normal = Vec3ScalarDivide(Temp2, Self->Radius);

                        return(true);
                }
        }

        return(false);
}

#endif /* SPHERE_C */
