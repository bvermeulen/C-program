// Week 3 submission - rational numbers
// Open and read a file of integer pairs into an array that is created with the first
// integer telling you how many to read.Each pair will be a struct rational - meaning
// it represents a rational number.So a struct will have two members - the numerator and
// the denominator.
//
// So 2 9 11 4 5 would mean create an rational array size 2 and read in the remaining 4
// values into data[]. Data[0] will be(9, 11) - the rational 9 / 11. Data[1] will be(4, 5) -
// the rational 4 / 5.
//
// Write four routines to add rationals, multiply rationals, subtract rationals and divide
// rationals.Take the file posted with this homework and produce the sum of all the rationals
// and the average of all the rationals in the file.


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAXNUM  100

typedef struct Rational {
  int nominator;
  int denominator;
} Rational;

int gcd();

void read_rationals(char *filename, Rational *q_ptr, int *size)
// read from filename first number of q numbers (size), then read size time
// the pair of number's nominator and denominator. Then devide both by the
// greatest common denominator. Assert denominator is not zero.
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
        assert(den != 0);
        gcd_q = gcd(abs(nom), abs(den));
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

Rational sum(Rational a, Rational b)
// calculate sum of two rational numbers. Negative numbers are allowed
{
    Rational result;
    result.denominator = a.denominator * b.denominator;
    result.nominator = (a.nominator * b.denominator + b.nominator * a.denominator);
    // gcd only works for positive integers
    int gcd_q = gcd(abs(result.nominator), abs(result.denominator));
    result.nominator /= gcd_q;
    result.denominator /= gcd_q;
    return result;
}

Rational subtract(Rational a, Rational b)
// calculate subtration of two rational numbers. Negative numbers are allowed
{
    Rational result;
    result.denominator = a.denominator * b.denominator;
    result.nominator = (a.nominator * b.denominator - b.nominator * a.denominator);
    // gcd only works for positive integers
    int gcd_q = gcd(abs(result.nominator), abs(result.denominator));
    result.nominator /= gcd_q;
    result.denominator /= gcd_q;
    return result;
}

Rational multiply(Rational a, Rational b)
// calculate multiplication of two rational numbers. Negative numbers are allowed
{
    Rational result;
    result.nominator = a.nominator * b.nominator;
    result.denominator = a.denominator * b.denominator;
    int gcd_q = gcd(abs(result.nominator), abs(result.denominator));
    result.nominator /= gcd_q;
    result.denominator /= gcd_q;
    return result;
}

Rational divide(Rational a, Rational b)
// calculate division of two rational numbers. Negative numbers are allowed
{
    Rational result;
    // devide by multiplying the inverse
    result.nominator = a.nominator * b.denominator;
    result.denominator = a.denominator * b.nominator;
    int gcd_q = gcd(abs(result.nominator), abs(result.denominator));
    result.nominator /= gcd_q;
    result.denominator /= gcd_q;
    return result;
}

int main(int argc, char *argv[])
{
    Rational q[MAXNUM], result, average, count;
    int size;

    // read the input file
    if (argc != 2)
    {
        printf("Provide filename with rational numbers ...\n");
        exit(1);
    }
    read_rationals(argv[1], q, &size);

    // print the results for +, -, x, /
    Rational num1 = q[0], num2 = q[1];
    printf("num1: (%-2d/%2d) [%6.3f]; ", num1.nominator, num1.denominator,
           (float) num1.nominator / (float) num1.denominator);
    printf("num2: (%-2d/%2d) [%6.3f]\n", num2.nominator, num2.denominator,
           (float)num2.nominator / (float)num2.denominator);

    result = sum(num1, num2);
    printf("(%-2d/%2d)  +  ((%-2d/%2d) = ", num1.nominator, num1.denominator,
           num2.nominator, num2.denominator);
    printf("(%-2d/%2d) [%6.3f]\n", result.nominator, result.denominator,
           (float)result.nominator / (float)result.denominator);

    result = subtract(num1, num2);
    printf("(%-2d/%2d)  -  ((%-2d/%2d) = ", num1.nominator, num1.denominator,
           num2.nominator, num2.denominator);
    printf("(%-2d/%2d) [%6.3f]\n", result.nominator, result.denominator,
           (float)result.nominator / (float)result.denominator);

    result = multiply(num1, num2);
    printf("(%-2d/%2d)  x  ((%-2d/%2d) = ", num1.nominator, num1.denominator,
           num2.nominator, num2.denominator);
    printf("(%-2d/%2d) [%6.3f]\n", result.nominator, result.denominator,
           (float)result.nominator / (float)result.denominator);

    result = divide(num1, num2);
    printf("(%-2d/%2d)  /  ((%-2d/%2d): ", num1.nominator, num1.denominator,
           num2.nominator, num2.denominator);
    printf("(%-2d/%2d) [%6.3f]\n", result.nominator, result.denominator,
           (float)result.nominator / (float)result.denominator);

    // calculate the average of all numbers
    printf("\nCalcultate the average ...\n");
    result.nominator = 0;
    result.denominator = 1;
    for (int i = 0; i < size; i++)
    {
        printf("number %2d: (%-2d/%2d) [%6.3f]\n", i + 1, q[i].nominator, q[i].denominator,
               (float)q[i].nominator / (float)q[i].denominator);
        result = sum(result, q[i]);
    }
    count.nominator = size;
    count.denominator = 1;
    result = divide(result, count);
    printf("The avarage is: (%-2d/%2d) [%6.3f]", result.nominator, result.denominator,
           (float)result.nominator / (float)result.denominator);

    return 0;
}
