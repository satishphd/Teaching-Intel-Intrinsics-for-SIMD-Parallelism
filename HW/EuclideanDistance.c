// C program to implement

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int SIZE = 255;

void read_ints (const char* file_name, int arr[])
{
  FILE* file = fopen (file_name, "r");
  
  if (NULL == file) 
  {
        printf("file can't be opened \n");
        exit(1);
  }
  int i = 0;
  int counter = 0;
  fscanf (file, "%d", &i);    
  while (!feof (file))
    {  
      //printf ("%d ", i);
      arr[counter] = i;
      fscanf (file, "%d", &i);  
      counter++;    
    }
  fclose (file);        
}

int euclidean_distance(int test[], int reference[], int SIZE)
{
  int counter;
  long int distance = 0;
  for(counter = 0; counter<SIZE; counter++)
  {
     distance += (test[counter] - reference[counter]) * (test[counter] - reference[counter]);
  }
  return distance;
}

// Driver code
int main()
{
    int test[SIZE];
    int reference[SIZE];
	read_ints("histograms/test/frog1.txt", test);
	
	read_ints("histograms/test/frog1.txt", reference);
	int dist1 = euclidean_distance(test, reference, SIZE);
	printf("Distance from itself = %d \n", dist1);
	
	read_ints("histograms/reference/frog2.txt", reference);
    dist1 = euclidean_distance(test, reference, SIZE);
	printf("Distance from frog2 = %d \n", dist1);
	
	read_ints("histograms/reference/frogRotated.txt", reference);
	dist1 = euclidean_distance(test, reference, SIZE);
	printf("Distance from rotated frog = %d \n", dist1);
		
	read_ints("histograms/reference/buck.txt", reference);
	dist1 = euclidean_distance(test, reference, SIZE);
	printf("Distance from buck = %d \n", dist1);

    read_ints("histograms/reference/marquette.txt", reference);
	dist1 = euclidean_distance(test, reference, SIZE);
	printf("Distance from marquette = %d \n", dist1);
	
	return 0;
}
