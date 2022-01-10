#include<stdio.h>
#include<immintrin.h>
#include<stdlib.h>
#include<sys/time.h>

// Compile: gcc -O3 -mavx512f  arrayLabAVX512.c
// Execute: ./a.out

/*
The following program shows two examples of translating C loops using AVX-512 intrinsics for SIMD parallelism.
*/

void vecAdd(int n, float *A, float *B, float *C)
{
  int i;
  for(i = 0; i<n; i++)
  {
    C[i] = C[i] + A[i] * B[i];
  }
}

// Implement vecAdd function using Intrinsic functions
void vecAddWithIntrinsics(int n, float *A, float *B, float *C)
{
  int i;
  for(i = 0; i<n; i=i+16)
  {
      __m512 vecA8 = _mm512_load_ps( &A[i] );
      __m512 vecB8 = _mm512_load_ps( &B[i] );
      __m512 temp = _mm512_mul_ps( vecA8, vecB8 );
      __m512 vecC8 = _mm512_load_ps( &C[i] );   
      __m512 temp2 = _mm512_add_ps( vecC8, temp );
      _mm512_store_ps( &C[i], temp2 ); 
  }
}


void vecAddV2(int n, double *A, double *B, double *C)
{
  int i,j;
  double a0,c0;

  for(i = 0; i<n; i++)
  {
    c0 = C[i];
    a0 = A[i];
    
    for(j = 0; j<n; j++)
    {
       c0 = c0 + a0 * B[j];
    }
    C[i] = c0;
  }
}

// Implement vecAddV2 using AVX512 intrinsic functions
void vecAddV2Intrinsics(int n, float *A, float *B, float *C)
{
  int i,j;

  __m512 a0, c0;
  
  for(i = 0; i<n; i = i+16)
  {
      c0 = _mm512_load_ps(C+i); 
      a0 = _mm512_load_ps(A+i); 
      
      for(j = 0; j<n; j = j+1)
      {
         float bj = B[j];
         __m512 bjVec = _mm512_set_ps( bj, bj, bj, bj,  bj, bj, bj, bj,  bj, bj, bj, bj,  bj, bj, bj, bj);
         c0 = _mm512_add_ps( c0, _mm512_mul_ps( a0, bjVec) );
      }     
     
      _mm512_store_ps( C+i , c0);
  }
}

// set a same value val to all the element of array arr
void set(float *arr, int length, float val)
{
    int i;
    for( i = 0; i<length; i++)
    {
       arr[i] = val;
    }
}

// initialize the array elements with different values
void initialize(float *arr, int length)
{
    int i;
    for( i = 0; i<length; i++)
    {
       arr[i] = i+1;
    }
}

void display(int n, float *arr)
{
    int i;
    // printf few values to check output
    for( i = 0; i < 16; i++)
    {
       printf("%.2f ", arr[i]);
       
       if(i%4 == 0)
        printf("\n");
    }
    printf("\n");
}

int main()
{
   
   //int NUM_ELEMENTS = 1024;
    int NUM_ELEMENTS = 1024*1024*128;
    //int NUM_ELEMENTS = 1024*128;
   
   printf("NUM_ELEMENTS %d, sizeof(float) %lu \n ", NUM_ELEMENTS, sizeof(float));  
 
   // memory allocation for arrays A, B, C using _mm_malloc
   size_t N_pd = NUM_ELEMENTS;

   float *data_A = (float*)_mm_malloc(N_pd*sizeof(float), 64);
   float *data_B = (float*)_mm_malloc(N_pd*sizeof(float), 64);
   float *data_C = (float*)_mm_malloc(N_pd*sizeof(float), 64);

   if(data_A == NULL || data_B == NULL || data_C == NULL)
   {
     printf("Error \n");
     return 1;
   }
  
   // initializing the arrays A, B and C
   initialize(data_A, NUM_ELEMENTS);
   initialize(data_B, NUM_ELEMENTS);
   initialize(data_C, NUM_ELEMENTS); 
   
   struct timeval  tv1, tv2;
   
   gettimeofday(&tv1, NULL);
   
   //vecAdd(NUM_ELEMENTS, data_A, data_B, data_C);
   
   vecAddWithIntrinsics(NUM_ELEMENTS, data_A, data_B, data_C);

   //vecAddV2Intrinsics(NUM_ELEMENTS, data_A, data_B, data_C);
 
   gettimeofday(&tv2, NULL);

   double time_spent = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                                     (double) (tv2.tv_sec - tv1.tv_sec); 
   
   printf("Intrinsics: Time %f \n", time_spent);

   display(NUM_ELEMENTS, data_C);

   // de-allocating memory for A, B, C using _mm_free
   _mm_free(data_A);
   _mm_free(data_B);
   _mm_free(data_C);
   
   return 0;
}
