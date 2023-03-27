// C++ program to implement
// g++ -o distance EuclideanDistance.cpp
// ./distance

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int SIZE = 255;

void read_ints (const char* file_name, double arr[])
{
  FILE* file = fopen (file_name, "r");
  
  if (NULL == file) 
  {
        printf("file can't be opened \n");
        exit(1);
  }
  double i = 0;
  int counter = 0;
  fscanf(file, "%lf", &i);    
  while (!feof (file))
    {  
      printf ("%.2f ", i);
      arr[counter] = i;
      fscanf (file, "%lf", &i);  
      counter++;    
    }
  fclose (file);        
}

double euclidean_distance(double test[], double reference[], int SIZE)
{
  int counter;
  double distance = 0;
  for(counter = 0; counter<SIZE; counter++)
  {
     distance += (test[counter] - reference[counter]) * (test[counter] - reference[counter]);
  }
  return distance;
}

// Driver code
int main()
{
    double test[SIZE];
    double reference[SIZE];
	read_ints("histograms/test/frog1.txt", test);
	
	read_ints("histograms/test/frog1.txt", reference);
	double dist1 = euclidean_distance(test, reference, SIZE);
	printf("Distance from itself = %f \n", dist1);
	
	read_ints("histograms/reference/frog2.txt", reference);
    dist1 = euclidean_distance(test, reference, SIZE);
	printf("Distance from frog2 = %f \n", dist1);
	
	read_ints("histograms/reference/frogRotated.txt", reference);
	dist1 = euclidean_distance(test, reference, SIZE);
	printf("Distance from rotated frog = %f \n", dist1);
		
	read_ints("histograms/reference/buck.txt", reference);
	dist1 = euclidean_distance(test, reference, SIZE);
	printf("Distance from buck = %f \n", dist1);

    read_ints("histograms/reference/marquette.txt", reference);
	dist1 = euclidean_distance(test, reference, SIZE);
	printf("Distance from marquette = %f \n", dist1);
	
	return 0;
}
