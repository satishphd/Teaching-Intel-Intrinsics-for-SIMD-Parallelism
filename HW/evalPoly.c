#include<stdio.h>
#include<stdlib.h>
#include<immintrin.h>
#include<math.h>

#define MAX 32

// Compile: gcc -O3 -mavx -o evalPoly evalPoly.c

// pow function in C: x to the power y is given by function pow(double x, double y);

// coefficients array is {1, 2, 3, 4, -2}
// polynomial: x^4 + 2x^3 + 3x^2 + 4x -2x^0
// Evaluating polynomial when x = 1,
// 1 + 2*1 + 3*1 + 4*1 -2
// 8  (ans)
double evaluate(double x, double *coefficients)
{
     int degree;
     
     double *terms = (double *)_mm_malloc( MAX * sizeof(double), 32);
     
     for(degree = 0; degree < MAX; degree++)
     {
           double powerOfX = pow(x, degree);
           
           // evaluate each term
           terms[degree] = coefficients[degree] * powerOfX;
     }
     
     double answer = 0;

     // sum all the terms 
     for(degree = 0; degree < MAX; degree++)
     {
            answer = answer + terms[degree];    
     }
     
     _mm_free(terms);
     
     return answer;
}

// Lab work: Implement using Intel Intrinsic functions
double evaluateSIMD(double x, double *coefficients)
{
    double answer = 0;
    // Loop, loop unrolling
    SIMDVec v
    SIMD_Load()
    SIMD_Store()
    SIMD_Mul()
    return answer;
}

// initialize the coefficients with different values
void initialize(double *coeffArr, int length)
{
    int i;
    for( i = 0; i<length; i++)
    {
       coeffArr[i] = i+1;
    }
}

int main()
{
   double x = 0.99999;
   
   size_t N_pd = (MAX * 8)/sizeof(double);

   double *coefficients = (double*)_mm_malloc(N_pd*sizeof(double), 32);

   initialize(coefficients, MAX);
   
   double answer = evaluate(x, coefficients);
   
   printf("#coefficients %lu Answer is %f \n", N_pd, answer);
   
   _mm_free(coefficients);
   
   return 0;
}
