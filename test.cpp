#include<immintrin.h>
#include<iostream>
#include<vector>

#include <math.h>

typedef double RectReal;      // float vs double

using namespace std;
using std::vector;

// g++ -O2 -mavx test.cpp

/*
The returned value of simd compare function is a bitmask: it is set to all zeroes for false, 
or all ones for true. 32 bits of ones happen to be encoding of NaN when interpreted as a 32-bit float.

Bitmasks are useful because you can use them to mask out some results, e.g. (A & M) | (B & ~M) 
will select the value of A when the mask M was true (all ones) and the value of B when the 
mask was false (all zeroes).

When greater than check returns true (a is gt than b) then the returned 32/64 bits are all
1s bitwise. This is treated as nan (not a number) because of IEEE-754 specification
thats why you will see nan check as shown below
isnan( gtResult[0] )      // https://cplusplus.com/reference/cmath/isnan/
*/

//0: xmin, 1: ymin, 2: xmax, 3: ymax
void rectUnion(vector<RectReal> *rect1, vector<RectReal> *rect2)
{
    rect1->at(0) = min( rect1->at(0), rect2->at(0) );
    rect1->at(1) = min( rect1->at(1), rect2->at(1) );
    rect1->at(2) = max( rect1->at(2), rect2->at(2) );
    rect1->at(3) = max( rect1->at(3), rect2->at(3) );
    cout<<"Inside rectUnion actual Union logic "<<endl;
    cout<<rect1->at(0)<<" "<<rect1->at(1) <<" "<< rect1->at(2) <<" "<< rect1->at(3)<<endl;
}

void testUnionSIMD( vector<RectReal> *rect1, vector<RectReal> *rect2 )
{
    cout<<"Inside testUnionSIMD function "<<endl;
    // xmin, ymin, xmax, ymax
    
    __m256d r1 = _mm256_set_pd( rect1->at(0), rect1->at(1), rect1->at(2), rect1->at(3) );
    
    __m256d r2 = _mm256_set_pd( rect2->at(0), rect2->at(1), rect2->at(2), rect2->at(3) );
    
    __m256d comparison = _mm256_cmp_pd(r1, r2, 14); //greater than comparison
    
    double gtResult[4];
    _mm256_store_pd( gtResult, comparison );
    
    cout<<gtResult[0]<<" "<<gtResult[1]<<" "<<gtResult[2]<<" "<<gtResult[3]<<endl;
    
    //cout<<"Is Not a number test "<<isnan( gtResult[0] )<<endl;
    //cout<<"Is Not a number test "<<isnan( gtResult[2] )<<endl;
        
    rect1->at(0) = (gtResult[3] == 0) ?  rect1->at(0) : rect2->at(0);  // if ( r1_minx is LTE r2_minx )
    rect1->at(1) = (gtResult[2] == 0) ?  rect1->at(1) : rect2->at(1);  // if ( r1_miny is LTE r2_miny )
    
    // isnan(x)  A non-zero value (true) if x is a NaN value; and zero (false) otherwise.
    
    rect1->at(2) = ( isnan( gtResult[1] ) ) ?  rect1->at(2) : rect2->at(2);
    rect1->at(3) = ( isnan( gtResult[0] ) ) ?  rect1->at(3) : rect2->at(3);
    
    cout<<rect1->at(0)<<" "<<rect1->at(1) <<" "<< rect1->at(2) <<" "<< rect1->at(3)<<endl;
    
}

void test1()
{
  vector<RectReal> *rect1 = new vector<RectReal>[4];
  rect1->push_back(1.0);      rect1->push_back(2.0);     rect1->push_back(3.0);      rect1->push_back(4.0);
    
  vector<RectReal> *rect2 = new vector<RectReal>[4]; 
  rect2->push_back(0.0);      rect2->push_back(0.0);     rect2->push_back(13.0);      rect2->push_back(14.0);

  
  rectUnion( rect1, rect2 );

  rect1 = NULL;
  rect2 = NULL;
  
  rect1 = new vector<RectReal>[4];
  rect1->push_back(1.0);      rect1->push_back(2.0);     rect1->push_back(3.0);      rect1->push_back(4.0);
    
  rect2 = new vector<RectReal>[4]; 
  rect2->push_back(0.0);      rect2->push_back(0.0);     rect2->push_back(13.0);      rect2->push_back(14.0);
  
  testUnionSIMD(  rect1, rect2 );

  //__m256d var = _mm256_set_pd( 1.1, 2.2, 3.3, 4.4); 
   // std::cout<<var[0]<<" "<<var[1]<<" "<<var[2]<<" "<<var[3];
   
   // prints 4.4 3.3 2.2 1.1         
   
}

void test2()
{
  vector<RectReal> *rect1 = new vector<RectReal>[4];
  rect1->push_back(1.0);      rect1->push_back(2.0);     rect1->push_back(3.5);      rect1->push_back(4.5);
    
  vector<RectReal> *rect2 = new vector<RectReal>[4]; 
  rect2->push_back(0.0);      rect2->push_back(3.0);     rect2->push_back(4.0);      rect2->push_back(6.0);

  rectUnion( rect1, rect2 );

  rect1 = NULL;
  rect2 = NULL;
  
  rect1 = new vector<RectReal>[4];
  rect1->push_back(1.0);      rect1->push_back(2.0);     rect1->push_back(3.5);      rect1->push_back(4.5);
    
  rect2 = new vector<RectReal>[4]; 
  rect2->push_back(0.0);      rect2->push_back(3.0);     rect2->push_back(4.0);      rect2->push_back(6.0);
  
  testUnionSIMD(  rect1, rect2 );

  //__m256d var = _mm256_set_pd( 1.1, 2.2, 3.3, 4.4); 
   // std::cout<<var[0]<<" "<<var[1]<<" "<<var[2]<<" "<<var[3];
   
   // prints 4.4 3.3 2.2 1.1         
   
}

void test3()
{
  vector<RectReal> *rect1 = new vector<RectReal>[4];
  rect1->push_back(1.0);      rect1->push_back(2.0);     rect1->push_back(5.0);      rect1->push_back(4.5);
    
  vector<RectReal> *rect2 = new vector<RectReal>[4]; 
  rect2->push_back(0.0);      rect2->push_back(3.0);     rect2->push_back(4.0);      rect2->push_back(6.0);

  rectUnion( rect1, rect2 );

  rect1 = NULL;
  rect2 = NULL;
  
  rect1 = new vector<RectReal>[4];
  rect1->push_back(1.0);      rect1->push_back(2.0);     rect1->push_back(5.0);      rect1->push_back(4.5);
    
  rect2 = new vector<RectReal>[4]; 
  rect2->push_back(0.0);      rect2->push_back(3.0);     rect2->push_back(4.0);      rect2->push_back(6.0);
  
  testUnionSIMD(  rect1, rect2 );

  //__m256d var = _mm256_set_pd( 1.1, 2.2, 3.3, 4.4); 
   // std::cout<<var[0]<<" "<<var[1]<<" "<<var[2]<<" "<<var[3];
   
   // prints 4.4 3.3 2.2 1.1         
   
}


int main()
{
  test1();
  
  cout<<"   Test 2 "<<endl;
  test2();
  
  cout<<"   Test 3 "<<endl;
  test3();
  
  return 0;
}