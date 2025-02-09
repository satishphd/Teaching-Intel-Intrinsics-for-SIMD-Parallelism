# Teaching-Intel-Intrinsics-for-SIMD-Parallelism
Teaching Vectorization and SIMD using Intel Intrinsics in a Computer Organization and Architecture class 

Intel Intrinsics Guide for Advanced Vector Extensions (AVX): https://software.intel.com/sites/landingpage/IntrinsicsGuide/#techs=AVX

My youtube lecture link on this topic of using SIMD parallelism using Intrinsics (1 hour lecture): https://www.youtube.com/watch?v=KABGmD7BJ28

0) vectorAdd.c is a sample program with x86 intrinsic functions that demonstrates how to use SIMD functions to add two input vectors to produce an ouput vector.

1) arrayLab.c is a program that can be assigned as lab work or homework for writing C loops using intrinsics

2) evalPoly.c is a program that contains a function to evaluate a polynomial. Lab work can be using intrinsics to implement the function.

3) matmul_intrinsic.c is a program that contains program that uses x86 AVX functions to implement matrix-matrix multiplication.
   loop_AVX512.c is a program that shows how to parallelize a C loop using AVX-512 intrinsic functions.

4) A pdf of the presentation slide is also there.

5) Problems for Quiz/Exam

I have covered broadcast, initialization, memory alignment allocation (static and dynamic ways) and matrix multiplication code using Intel Intrinsics in Part 2.

This document describes the intrinsic functions to implement the homework and lab:

1) __m256d _mm256_load_pd( double const *mem_addr )

Description: Load 256-bits (composed of 4 packed double-precision (64-bit) floating-point 
elements) from memory into a variable of type __m256d and then return this variable. 
mem_addr must be aligned on a 32-byte boundary or code can crash due to segmentation fault.
Memory alignment is ensured using mm_malloc() function.

**Aligned memory allocation using _mm_malloc function**
 
 void* _mm_malloc (int sizeInBytes, int alignment) // alignment is 32 for AVX intrinsics
 
 Example Usage: Allocate memory for 128 doubles or an array of doubles of size 128.
 size_t N = 128;   
 double *arr = (double *)_mm_malloc( N * sizeof(double), 32 );
 

2) void _mm256_store_pd( double *mem_addr, __m256d a )

Description: Store 256-bits (composed of 4 packed double-precision (64-bit) floating-point
elements) from argument a of the store function into memory. mem_addr must be aligned on a
32-byte boundary or code can crash with segmentation fault. Memory alignment is ensured
using mm_malloc() function.


3) __m256d _mm256_add_pd( __m256d a, __m256d b )

Description: Add packed double-precision (64-bit) floating-point elements in a and b, and 
returns the sum.


4) __m256d _mm256_mul_pd( __m256d a, __m256d b )

Multiply packed double-precision (64-bit) floating-point elements in a and b,  and returns
the product.


5) __m256d _mm256_set_pd( double e3, double e2, double e1, double e0 )

Initialize a __m256d variable with four doubles passed to this function (e0, e1, e2, e3) 
and return it back.

6) __m256d _mm256_cmp_pd(r1, r2, 14); //greater than comparison     
   Returns bitmask (1111s or 0000s) that stores the comparison result of this condition if( r1 > r2 ).
   
   if (r1 > r2 ), returns 1111..      (Since this is bitwise 1111.., this is NaN in IEEE-754 specification) 
   
   if (r1 > r2 ), returns 0000.. 
   
7)   __m256d _mm256_and_pd(__m256d m1, __m256d m2)

      Performs bitwise logical AND operation on float64 vectors.
     
     Verification:
     cout<<"ifVec    "<<ifVec[0]<<" "<<ifVec[1]<<" "<<ifVec[2]<<" "<<ifVec[3]<<endl;     
   
 9) __m256d _mm256_andnot_pd(__m256d m1, __m256d m2)

     Performs bitwise logical AND NOT operation on float64 vectors
       
     Verification:
     cout<<"elseVec  "<<elseVec[0]<<" "<<elseVec[1]<<" "<<elseVec[2]<<" "<<elseVec[3]<<endl;
          
10)  __m256d _mm256_or_pd( __m256d m1, __m256d m2);

      Performs bitwise logical OR operation on float64 vectors.

       Verification:
       cout<<"simdCMP    "<<xV[0]<<" "<<xV[1]<<" "<<xV[2]<<" "<<xV[3]<<endl;
