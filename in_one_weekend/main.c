#include <stdio.h>
#include <stdlib.h>

int
main(int ArgCount, char **Arguments)
{
        int X = 200;
        int Y = 100;

        printf("P3\n%i %i\n255\n", X, Y);

        for(int J = Y-1; J >= 0; J--)
        {
                for(int I = 0; I < X; I++)
                {
                        float R = (float)I / (float)X;
                        float G = (float)J / (float)Y;
                        float B = 0.2;

                        int RInt = (int)(255.99 * R);
                        int GInt = (int)(255.99 * G);
                        int BInt = (int)(255.99 * B);

                        printf("%i %i %i\n", RInt, GInt, BInt);
                }
        }

        return(EXIT_SUCCESS);
}
