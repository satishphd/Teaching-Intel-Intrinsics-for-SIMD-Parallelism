#include<stdio.h>
#include<immintrin.h>
#include<stdlib.h>
#include<sys/time.h>


//#define SIZE 1024
#define SIZE 64

// compile: gcc -O3 -o prog -mavx matmul_intrinsic.c
// run:     ./prog

 void dgemmIntrin(int n, double *A, double *B, double *C)
{
  int i, j, k;
  
  for (  i = 0; i < n; i = i+4) {
        for (  j = 0; j < n; j++)  {
           __m256d c0 = _mm256_load_pd( C + i + j*n);    // c0 = C[i][j]

            for(  k = 0; k < n; k++) {
               c0 = _mm256_add_pd( c0,  
                    _mm256_mul_pd ( _mm256_load_pd( A + i + k*n),
                    _mm256_broadcast_sd( B + k + j*n ) ) );
            }        
           _mm256_store_pd( C + i + j*n, c0);           // C[i][j] = c0        
       } 
   }
} 

void dgemm(int n, double *A, double *B, double *C)
{
   int i, j, k;
   
   for( i = 0; i<n; i++)
   {
     for( j = 0; j < n; j++)
     {
         double cij = C[i + j*n];
         for( k = 0; k<n; k++)
         {
            cij = cij + A[i + k*n] * B[k + j*n];
         }
         C[i + j*n] = cij;
     }
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

void initializeMM(double *arr, int length)
{
    int i;
    for( i = 0; i<length; i++)
    {
       arr[i] = i;
    }
}

void printMatrix(double *arr, int numElements, int dimension)
{
   int i;
   
   for( i = 0; i < numElements; i++)
   {
      if(i%SIZE == 0)
        printf("\n");
      printf("%f ", arr[i]);
   }
}

int matmul_intrinsics()
{ 

   int NUM_ELEMENTS = SIZE*SIZE;
   int dimension = SIZE;
   printf("dimension = %d & NUM_ELEMENTS %d, sizeof(double)=%lu \n ", dimension, NUM_ELEMENTS, sizeof(double));
   
   size_t N_pd = (NUM_ELEMENTS*8)/sizeof(double);

   double *data_A = (double*)_mm_malloc(N_pd*sizeof(double), 32);
   double *data_B = (double*)_mm_malloc(N_pd*sizeof(double), 32);
   double *data_C = (double*)_mm_malloc(N_pd*sizeof(double), 32);

   if(data_A == NULL || data_B == NULL || data_C == NULL)
   {
     printf("Error \n");
     return 1;
   }
   
   initialize(data_A, NUM_ELEMENTS, 1.0);
   //initialize(data_B, NUM_ELEMENTS, 2.0);
   initializeMM(data_B, NUM_ELEMENTS);
   initialize(data_C, NUM_ELEMENTS, 0.0);
   
   struct timeval  tv1, tv2;
   
   gettimeofday(&tv1, NULL);
   
   dgemmIntrin(dimension, data_A, data_B, data_C);

   gettimeofday(&tv2, NULL);

   double time_spent = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                                     (double) (tv2.tv_sec - tv1.tv_sec); 
   
   printf("Intrinsics: Time %f \n", time_spent);
   
   double numOps = 2.0 * SIZE * SIZE * SIZE;

   double gflops = 1.0e-9 * numOps / time_spent;

   printf(" Intrinsics: numOps = %.2f GFLOPS %f \n", numOps, gflops);
   
   //printMatrix(data_C, NUM_ELEMENTS, dimension);  
   
   _mm_free(data_A);
   _mm_free(data_B);
   _mm_free(data_C);

   return 0;
}

int matmul()
{
   double *A, *B, *C;
   int dimension = SIZE;
   int numElements = SIZE * SIZE;
   printf("dimension = %d & numElements %d \n ", dimension, numElements);
   
   A = (double *)malloc(sizeof(double) * numElements );
   initialize(A, numElements, 1.0);

   B = (double *)malloc(sizeof(double) * numElements );
   //initialize(B, numElements, 1.0);
   initializeMM(B, numElements);
   
   C = (double *)malloc(sizeof(double) * numElements );
   initialize(C, numElements, 0.0);
   
   struct timeval  tv1, tv2;
   
   gettimeofday(&tv1, NULL);
   
   dgemm(dimension, A, B, C);

   gettimeofday(&tv2, NULL);

   double time_spent = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
                                     (double) (tv2.tv_sec - tv1.tv_sec);
    
   printf("  Naive: Time %f \n", time_spent);
   //printMatrix(C, numElements, dimension);  
   
   double numOps = 2.0 * SIZE * SIZE * SIZE;

   double gflops = 1.0e-9 * numOps / time_spent;

   printf("   Naive: numOps = %.2f GFLOPS %f \n", numOps, gflops);
    
   free(A);
   free(B);
   free(C);
   
   return 0;
}

int main()
{
   printf("Normal Matmul and then Mat Mul using Intel Intrinsics \n");

   matmul();
   
   matmul_intrinsics();
   
   return 0;
}