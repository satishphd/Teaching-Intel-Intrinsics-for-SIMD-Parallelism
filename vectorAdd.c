#include<stdio.h>
#include<immintrin.h>
#include<stdlib.h>
#include<sys/time.h>

// Compile: gcc -O3 -mavx -o arrayLab vectorAdd.c
// Execute: ./arrayLab

void vecAdd(int n, double *A, double *B, double *C)
{
  int i;
  for(i = 0; i<n; i = i+4)
  {
    C[i] = A[i] + B[i];
    C[i+1] = A[i+1] + B[i+1];
    C[i+2] = A[i+2] + B[i+2];
    C[i+3] = A[i+3] + B[i+3];
  }
}
// Lab Work: Implement vecAdd function using Intrinsic functions
void vecAddWithIntrinsics(int n, double *A, double *B,
 double *C)
{
  int i;
  for(i = 0; i<n; i = i+4)
  {
     __m256d aVec = _mm256_load_pd( &A[i] );
     __m256d bVec = _mm256_load_pd( &B[i] );
     __m256d cVec = _mm256_add_pd( aVec, bVec );
     _mm256_store_pd( &C[i], cVec ); 
  }
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

void initialize(double *arr, int length, double val)
{
    int i;
    for( i = 0; i<length; i++)
    {
       arr[i] = val;
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
   //int NUM_ELEMENTS = 32;
   int NUM_ELEMENTS = 1024*1024*128;
   
   printf("NUM_ELEMENTS %d, sizeof(double) %lu \n ", NUM_ELEMENTS, sizeof(double));
   
   double *data_A = (double*)_mm_malloc( NUM_ELEMENTS * sizeof(double), 32);
   double *data_B = (double*)_mm_malloc( NUM_ELEMENTS * sizeof(double), 32);
   double *data_C = (double*)_mm_malloc( NUM_ELEMENTS * sizeof(double), 32);

   if(data_A == NULL || data_B == NULL || data_C == NULL)
   {
     printf("Error \n");
     return 1;
   }
   
   initialize(data_A, NUM_ELEMENTS, 1.0);
   initialize(data_B, NUM_ELEMENTS, 2.0);
   initialize(data_C, NUM_ELEMENTS, 0.0);
   
   // Measure execution time elapsed by function
   struct timeval  tv1, tv2;
   
   gettimeofday(&tv1, NULL);
   
   vecAdd(NUM_ELEMENTS, data_A, data_B, data_C);
   
   //vecAddWithIntrinsics(NUM_ELEMENTS, data_A, data_B, data_C);
   
   double time_spent = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                                     (double) (tv2.tv_sec - tv1.tv_sec); 
   
   printf("Execution Time %f \n", time_spent);

   display(NUM_ELEMENTS, data_C);
   // de-allocating memory for A, B, C using _mm_free
   
   return 0;
}