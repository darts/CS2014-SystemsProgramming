/* Program that reads in a text file of integers
   and outputs the numbers in sorted order. */

#include <stdio.h>
#include <stdlib.h>

/* find smallest element between start and end of array */
int[] findMin(int a[], int size, int start) {
  int min = a[start];
  int result[0] = start;
  result[1] = 0;//counter variable
  int i;

  for ( i = start; i < size; i++ ) {
    if ( a[i] < min ) {
      min = a[i];
      result[0] = i;
    }
    result[1]++; //increment counter
  }
  return result;
}

/* selection sort algorithm: repeatedly find smallest
   element and place at start of unsorted section. */
int selectionSort(int a[], int size) {
  int count = 0;
  for(int i = 0; i < size; i++){
    count++;
    int[] minLoc = findMin(a, size, i); // find the lowest number
    count += minLoc[1];
    int min = a[minLoc[0]];
    int tmp = a[i];
    a[i] = min; //swap the two numbers
    a[minLoc[0]] = tmp;
  }
  return count;
}

/* insertion sort algorithm: place unsorted array
   elements into the correct location in an initially
   empty sorted part.*/
int insertionSort(int a[], int size) {
  int count = 0;
  for(int i = 1; i < size; i++){//loop through array
  count++;
    int numToSort = a[i];
    int hasFinished = 0; //boolean value
    for(int j = i - 1; j >= 0 && hasFinished == 0; j--){ //shift elements of the array
      count++;
      int prevNum = a[j];
      if(prevNum > numToSort){//swap elements if needed
        a[j] = numToSort;
        a[j + 1] = prevNum;
      }else{
        hasFinished = 1;
      }
    }
  }
  return count;
}

/* bubble sort algorithm: repeatedly compare and swap
   adjacent array elements. */
int bubbleSort(int a[], int size) {
  int count = 0;
  for(int i = 0; i < size; i++){//loop through array
    count++;
    for(int j = i; j < size - 1; j++){ //shift elements of the array
      count++;
      int currentNum = a[j];
      int nextNum = a[j + 1];
      if(currentNum > nextNum){//swap elements if needed
        a[j + 1] = currentNum;
        a[j] = nextNum;
      }
    }
  }
  return count;
}

/* funny sort algorithm: like bubble sort only we
   go back to the start every time we find a pair
   out of order. */
void funnySort(int a[], int size) {
  int j, temp;

  j = 0;
  while ( j < size - 1 ) {
    if ( a[j] > a[j+1] ) {
      temp = a[j];
      a[j] = a[j+1];
      a[j+1] = temp;
      j = 0;
    }
    else {
      j++;
    }
  }
}

/* Open a file. Abort progam if file cannot be opened */
FILE * open_file(char filename[])
{
  FILE * file;

  file = fopen(filename, "r");
  if ( file == NULL ) {
    printf("FATAL: Error opening file %s. Aborting program.\n", filename);
    exit(1);
  }

  return file;
}

/* read a stream of up to 'size' integers from a text file.
   return number of integers */
int read_in(int a[], int size, char filename[])
{
  const int max_line = 1024;
  char line[max_line];
  int i;
  FILE * file;
  char * eof;

  file = open_file(filename);

  /* read in the ints - one per line */
  i = 0;
  eof = fgets(line, max_line, file);
  while ( eof != NULL && i < size ) {     /* eof == NULL => end of file */
    sscanf(line, "%d", &(a[i]));
    i++;
    eof = fgets(line, max_line, file);
  }
  fclose(file);
  return i;
}

/* write out an array of integers up to 'size' */
void write_out(int a[], int size)
{
  int i;

  for ( i = 0; i < size; i++ ) {
    printf("%d\n", a[i]);
  }
}


/* read in a file of numbers, sort them, and
   write them out to the screen */
int main()
{
  const int size = 1024;
  int nums[size];
  int nnums;
  int count = 0;

  //funnySort
  nnums = read_in(nums, size, "numbers.txt");
  funnySort(nums, nnums);
  write_out(nums, nnums);
  /* flush output buffer, so all output appears on screen */
  fflush(stdout);

  //selectionSort
  nnums = read_in(nums, size, "numbers.txt");
  count = selectionSort(nums, nnums);
  write_out(nums, nnums);
  /* flush output buffer, so all output appears on screen */
  fflush(stdout);
  printf("%d loop iterations.SelectionSort \n", count);

  //insertionSort
  nnums = read_in(nums, size, "numbers.txt");
  count = insertionSort(nums, nnums);
  write_out(nums, nnums);
  /* flush output buffer, so all output appears on screen */
  fflush(stdout);
  printf("%d loop iterations. InsertionSort \n", count);


  //bubbleSort
  nnums = read_in(nums, size, "numbers.txt");
  count = bubbleSort(nums, nnums);
  write_out(nums, nnums);
  /* flush output buffer, so all output appears on screen */
  fflush(stdout);
  printf("%d loop iterations. BubbleSort \n", count);

  return 0;
}