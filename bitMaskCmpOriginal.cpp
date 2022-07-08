#include<immintrin.h>
#include<iostream>

using namespace std;

/*
    This program is written by me to illustrate how to vectorize if/else statement 
    using explicit vectorization
    
    Although I wrote this example, 
    the orginial credit goes to the video tutorial which I saw first and 
    then expanded on the professor's example here.
    
    SIMD Compare instruction discussed at at time 36:18 to 42:01
    https://www.youtube.com/watch?v=AT5nuQQO96o&t=2178s
*/

void sequentialCmp(  double a[],   double b[], int size )
{
  double x[4];
  for(int i = 0; i<size; i++)
  {
    if( a[i] > b[i] )  
    {
       x[i] = a[i];
    }
    else
    {
       x[i] = b[i]; 
    }
  }
  cout<<"sequential "<<x[0]<<" "<<x[1]<<" "<<x[2]<<" "<<x[3]<<endl;
  //x = {1, 2, 3, 14}
}

/* This is parallelization of the function sequentialCmp as shown above */
void simdCMP(  double a[],   double b[], int size )
{
     __m256d aVec = _mm256_load_pd( &a[0] );
     __m256d bVec = _mm256_load_pd( &b[0] );
     
     __m256d comparison = _mm256_cmp_pd( aVec, bVec, 14); //greater than comparison     
     
     __m256d ifVec = _mm256_and_pd(comparison, aVec);
     cout<<"ifVec    "<<ifVec[0]<<" "<<ifVec[1]<<" "<<ifVec[2]<<" "<<ifVec[3]<<endl;
     
     __m256d elseVec = _mm256_andnot_pd(comparison, bVec);
     cout<<"elseVec  "<<elseVec[0]<<" "<<elseVec[1]<<" "<<elseVec[2]<<" "<<elseVec[3]<<endl;
          
     __m256d xV = _mm256_or_pd(ifVec, elseVec);
     cout<<"simdCMP    "<<xV[0]<<" "<<xV[1]<<" "<<xV[2]<<" "<<xV[3]<<endl;

      //For verification, xV = { 1, 2, 3, 14 }
}

int main()
{
  double a[] = {1.0, 0.0, 1.0, 14.0}; 
  double b[] = {0.0, 2.0, 3.0,  4.0}; 
  
  sequentialCmp( a, b, 4 );
  
  simdCMP( a, b, 4 );
  
  return 0;
}
