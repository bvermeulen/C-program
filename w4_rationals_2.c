#include <stdio.h>
#include <stdlib.h>

#define MAXNUM  100

typedef struct Rational {
  int nominator;
  int denominator;
} Rational;

int gcd();

void read_rationals(char *filename, Rational *q_ptr, int *size)
// read from filename first number of q numbers (size), then read size time
// pair of number nominator and denominator.
// Then devide both by the greatest common denominator
{
    int nom, den, gcd_q;
    FILE *ifp;

    ifp = fopen(filename, "r");
    rewind(ifp);
    fscanf(ifp, "%d", &(*size));
    printf("size is: %d\n", *size);

    for (int i = 0; i < *size; i++)
    {
        fscanf(ifp, "%d %d", &nom, &den);
        printf("nominator: %3d, denominator: %3d\n", nom, den);
        gcd_q = gcd(nom, den);
        q_ptr->nominator = nom / gcd_q;
        q_ptr->denominator = den / gcd_q;
        q_ptr++;
    }
    fclose(ifp);
}

int gcd(int a, int b)
// calculation of the greatest common denominator by recursion
{
    // Everything divides 0
    if (a == 0) return b;
    if (b == 0) return a;

    // base case
    if (a == b) return a;

    // a is greater
    if (a > b) return gcd(a - b, b);
    // a is smaller
    else return gcd(a, b - a);
}

int main(int argc, char *argv[])
{
    Rational q[MAXNUM];
    Rational *q_ptr = NULL;
    q_ptr = q;
    int *size, a = 0;
    size = &a;

    if (argc != 2)
    {
        printf("Provide filename with rational numbers ...\n");
        exit(1);
    }

    read_rationals(argv[1], q_ptr, size);

    for (int i = 0; i < *size; i++)
    {

      printf("Rational: %3d / %3d = %10.5f\n", q[i].nominator, q[i].denominator,
             (float) q[i].nominator / (float) q[i].denominator);
    }
}
