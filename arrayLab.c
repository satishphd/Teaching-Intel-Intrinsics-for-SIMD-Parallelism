#include<stdio.h>
#include<immintrin.h>
#include<stdlib.h>
#include<sys/time.h>

// Compile: gcc -O3 -mavx -o arrayLab arrayLab.c
// Execute: ./arrayLab

void vecAdd(int n, double *A, double *B, double *C)
{
  int i;
  for(i = 0; i<n; i++)
  {
    C[i] = C[i] + A[i] * B[i];
  }
}

// Lab Work: Implement vecAdd function using Intrinsic functions
void vecAddWithIntrinsics(int n, double *A, double *B, double *C)
{
  
}

// set a same value val to all the element of array arr
void set(double *arr, int length, double val)
{
    int i;
    for( i = 0; i<length; i++)
    {
       arr[i] = val;
    }
}

// initialize the array elements with different values
void initialize(double *arr, int length)
{
    int i;
    for( i = 0; i<length; i++)
    {
       arr[i] = i+1;
    }
}

void display(int n, double *arr)
{
    int i;
    // printf few values to check output
    for( i = 0; i < 16; i++)
    {
       printf("%.2f ", arr[i]);
    }
    printf("\n");
}

int main()
{
   // memory allocation for arrays A, B, C using _mm_malloc
   int NUM_ELEMENTS = 1024;
   // int NUM_ELEMENTS = 1024*1024*128;
   
   printf("NUM_ELEMENTS %d, sizeof(double) %lu \n ", NUM_ELEMENTS, sizeof(double));
   
   double *A;
   double *B;
   double *C;
   
   // initializing the arrays A, B and C
   
   
   
   // Measure execution time elapsed by function
   struct timeval  tv1, tv2;
   
   gettimeofday(&tv1, NULL);
   
   //vecAdd(NUM_ELEMENTS, A, B, C);
   
   vecAddWithIntrinsics(NUM_ELEMENTS, A, B, C);
   
   double time_spent = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                                     (double) (tv2.tv_sec - tv1.tv_sec); 
   
   printf("Execution Time %f \n", time_spent);


   // de-allocating memory for A, B, C using _mm_free
   
   return 0;
}