#include<immintrin.h>
#include<iostream>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <utility>

// Whether two rectangles overlap or not?
// https://www.geeksforgeeks.org/find-two-rectangles-overlap/
// We assume lower left and upper right corners of rectangle

// g++ -O2 -mavx simdOverlap.cpp

using namespace std;
using std::vector;

typedef double RectReal;      // float vs double

void test1();
void test2();
void test3();
void test4();
void test5();
void test6();

int doOverlap( struct MBR *R1, struct MBR *R2 );

struct MBR   
{                // original array of vertices of a polygon
    vector<RectReal> *boundary; /* xmin,ymin,...,xmax,ymax,... */
    
    MBR( vector<RectReal> *c )
    {
        boundary = c;
    }
};

/* Based on code written by research assistant Ulises Nevarez */
int simdDoOverlap( struct MBR *R1, struct MBR *R2 )
{
    //0: xmin, 1: ymin, 2: xmax, 3: ymax
    __m256d R1MBR1 = _mm256_set_pd(R1->boundary->at(3), R1->boundary->at(2), R1->boundary->at(1), R1->boundary->at(0));
    __m256d R2MBR1 = _mm256_set_pd(R2->boundary->at(1), R2->boundary->at(0), R2->boundary->at(3), R2->boundary->at(2));
    __m256d comparison = _mm256_cmp_pd(R1MBR1, R2MBR1, 14); //greater than comparison

    if ( isnan(comparison[0]) ||    // xmin > xmax2  rectangle 1 is on the right side of the other rectangle
         isnan(comparison[1]) ||    // ymin > ymax2  rectangle 1 is on the top of the other rectangle
         (comparison[2] == 0) ||    // xmax <= xmin2 rectangle 1 is on the left side of the other rectangle
         (comparison[3] == 0) )     // ymax <= ymin2 rectangle 1 is below the other rectangle 
    {
        return 0;       // no overlap
    }
    else
    {
        return 1;       // overlaps
    }
}

/* This method is sequential code. 
   This is written to illustrate how to explicity vectorize overlap test on two rectangles. 
   Returns 1 for overlap; else return 0 */
int doOverlap( struct MBR *R1, struct MBR *R2 )
{  
    double xmin = R1->boundary->at(0);
    double ymin = R1->boundary->at(1);
    double xmax = R1->boundary->at(2);
    double ymax = R1->boundary->at(3);
    
    double xmin2 = R2->boundary->at(0);
    double ymin2 = R2->boundary->at(1);
    double xmax2 = R2->boundary->at(2);
    double ymax2 = R2->boundary->at(3);

    // If one rectangle is on left side of other
    if( xmin > xmax2 || xmin2 > xmax )
       return 0;
        
    // If one rectangle is above other
    if( ymax < ymin2 || ymax2 < ymin )
       return 0;
 
    return 1;
}

int main()
{
  test1();
  
  test2();

  test3();
  
  test4();

  test5();    
  
  test6();    
  
  return 0;
}

void test1()
{
  struct MBR *t1 = (struct MBR *)malloc(sizeof(struct MBR));
  t1->boundary = new vector<RectReal>[4];     
  t1->boundary->push_back(1.0);
  t1->boundary->push_back(2.0);
  t1->boundary->push_back(3.0);
  t1->boundary->push_back(4.0);

  struct MBR *t2 = (struct MBR *)malloc(sizeof(struct MBR));
  t2->boundary = new vector<RectReal>[4];     
  t2->boundary->push_back(0.0);
  t2->boundary->push_back(0.0);
  t2->boundary->push_back(13.0);
  t2->boundary->push_back(14.0);

  int result = simdDoOverlap(t1, t2);
  cout<<"Sequential test1: (expected 1) " << doOverlap( t1, t2 )<<" "<<result<<endl;
}

void test2()
{
  struct MBR *t1 = (struct MBR *)malloc(sizeof(struct MBR));
  t1->boundary = new vector<RectReal>[4];     
  t1->boundary->push_back(1.0);
  t1->boundary->push_back(2.0);
  t1->boundary->push_back(3.0);
  t1->boundary->push_back(4.0);

  struct MBR *t2 = (struct MBR *)malloc(sizeof(struct MBR));
  t2->boundary = new vector<RectReal>[4];     
  t2->boundary->push_back(10.0);
  t2->boundary->push_back(10.0);
  t2->boundary->push_back(13.0);
  t2->boundary->push_back(14.0);

  int result = simdDoOverlap(t1, t2);
  cout<<"Sequential test2 (expected 0) " << doOverlap( t1, t2 )<<" "<<result<<endl;
}

void test3()
{
  struct MBR *t1 = (struct MBR *)malloc(sizeof(struct MBR));
  t1->boundary = new vector<RectReal>[4];     
  t1->boundary->push_back(1.0);
  t1->boundary->push_back(2.0);
  t1->boundary->push_back(3.0);
  t1->boundary->push_back(4.0);

  struct MBR *t2 = (struct MBR *)malloc(sizeof(struct MBR));
  t2->boundary = new vector<RectReal>[4];     
  t2->boundary->push_back(10.0);
  t2->boundary->push_back(10.0);
  t2->boundary->push_back(13.0);
  t2->boundary->push_back(14.0);

  int result = simdDoOverlap(t2, t1);
  cout<<"Sequential test3 (expected 0): " << doOverlap( t2, t1 )<<" "<<result<<endl;
}

void test4()
{
  struct MBR *t1 = (struct MBR *)malloc(sizeof(struct MBR));
  t1->boundary = new vector<RectReal>[4];     
  t1->boundary->push_back(1.0);
  t1->boundary->push_back(2.0);
  t1->boundary->push_back(3.0);
  t1->boundary->push_back(4.0);

  struct MBR *t2 = (struct MBR *)malloc(sizeof(struct MBR));
  t2->boundary = new vector<RectReal>[4];     
  t2->boundary->push_back(0.0);
  t2->boundary->push_back(0.0);
  t2->boundary->push_back(13.0);
  t2->boundary->push_back(14.0);

  int result = simdDoOverlap(t2, t1);
  cout<<"Sequential test4: (expected 1) " << doOverlap( t2, t1 )<<" "<<result<<endl;
}

void test5()
{
  struct MBR *t1 = (struct MBR *)malloc(sizeof(struct MBR));
  t1->boundary = new vector<RectReal>[4];     
  t1->boundary->push_back(1.0);
  t1->boundary->push_back(2.0);
  t1->boundary->push_back(7.0);
  t1->boundary->push_back(7.0);

  struct MBR *t2 = (struct MBR *)malloc(sizeof(struct MBR));
  t2->boundary = new vector<RectReal>[4];     
  t2->boundary->push_back(3.0);
  t2->boundary->push_back(4.0);
  t2->boundary->push_back(5.0);
  t2->boundary->push_back(5.0);

  int result = simdDoOverlap(t1, t2);
  cout<<"Sequential test5: (expected 1) " << doOverlap( t1, t2 )<<" "<<result<<endl;
}

void test6()
{
  struct MBR *t1 = (struct MBR *)malloc(sizeof(struct MBR));
  t1->boundary = new vector<RectReal>[4];     
  t1->boundary->push_back(1.0);
  t1->boundary->push_back(2.0);
  t1->boundary->push_back(7.0);
  t1->boundary->push_back(7.0);

  struct MBR *t2 = (struct MBR *)malloc(sizeof(struct MBR));
  t2->boundary = new vector<RectReal>[4];     
  t2->boundary->push_back(3.0);
  t2->boundary->push_back(4.0);
  t2->boundary->push_back(5.0);
  t2->boundary->push_back(5.0);

  int result = simdDoOverlap(t2, t1);
  cout<<"Sequential test5: (expected 1) " << doOverlap( t2, t1 )<<" "<<result<<endl;
}
