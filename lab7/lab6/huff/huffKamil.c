// code for a huffman coder
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "huff.h"

//function headers
void makeCompound(struct huffchar* smallest, struct huffchar* scndSmallest, struct huffchar * compound, int count);
int findMin(struct huffchar* arr[], int index);

// create a new huffcoder structure
struct huffcoder *  huffcoder_new()
{
    struct huffcoder* result;
    result = malloc(sizeof(struct huffcoder));
    //result = malloc(sizeof(struct huffcoder)+sizeof(struct huffchar) * NUM_CHARS);
    return result;
}

// count the frequency of characters in a file; set chars with zero
// frequency to one
void huffcoder_count(struct huffcoder * this, char * filename)
{
    // set all the frequencies to 0
    for(int i=0; i < NUM_CHARS; i++){
     this->freqs[i] = 0;
    }
    int tmp=0;

  unsigned char c;  // we need the character to be unsigned to use it as an index
  FILE * file;
  file = fopen(filename, "r");
  assert( file != NULL );
  c = fgetc(file);	// attempt to read a byte
  while( !feof(file) ) {
    tmp = (unsigned)c;
    if(tmp>=0 && tmp<256){
        this->freqs[tmp]++;
    }
    c = fgetc(file);
  }
    // set all the frequencies to 0
    for(int i=0; i < NUM_CHARS; i++){
     if(this->freqs[i]==0){
        this->freqs[i] = 1;
     }
    }
  fclose(file);
}


// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes
void huffcoder_build_tree(struct huffcoder * this)
{
    struct huffchar* charList[NUM_CHARS];
    for(int i=0; i<NUM_CHARS;i++){
        struct huffchar * theChar = malloc(sizeof(struct huffchar));
        //charList[i]=(struct huffchar *) calloc(1, sizeof(struct huffchar));
        charList[i] = theChar;
        charList[i]->freq = this->freqs[i];
        charList[i]->is_compound=0;
        charList[i]->u.c=i;
        charList[i]->seqno=i;
    }

    int counter = NUM_CHARS - 1;
    for(int i=0; i<NUM_CHARS-1; i++){
        // find index of smallest frequency value
        int smallestIndex = findMin(charList, counter);
        //swap [counter] with smallest
        struct huffchar* tmpMin = malloc(sizeof(struct huffchar));
        tmpMin = charList[smallestIndex];
        struct huffchar* tmp = malloc(sizeof(struct huffchar));;
        tmp = charList[counter];
        charList[counter]=tmpMin;
        charList[smallestIndex]=tmp;
        // find index of second smallest freuqency value
        int scndSmallestIndex = findMin(charList, counter-1);
        //swap [counter-1] with scndSmallest
        struct huffchar* tmpScndMin = malloc(sizeof(struct huffchar));
        struct huffchar * tmp2 = malloc(sizeof(struct huffchar));
        tmpScndMin = charList[scndSmallestIndex];
        tmp2=charList[counter-1];
        charList[counter-1]=tmpScndMin;
        charList[scndSmallestIndex]=tmp2;


        // make compound char of smallest and scndSmallest
        struct huffchar* compound = malloc(sizeof(struct huffchar));
        makeCompound(charList[counter], charList[counter-1], compound, i);
        // update left and right children
        printf("%d %d %d %d\n", charList[counter]->freq,charList[counter-1]->freq,charList[counter]->seqno, charList[counter-1]->seqno );
        // make second last value of array the compound node just made
        charList[counter-1]=compound;
        // remove last value of array
        charList[counter]=NULL;
        counter--;
    }
    // pass pointer to root of tree
    printf("%d\n", counter);
    this->tree = charList[counter];
    printf("%d\n", this->tree->seqno);
}


// recursive function to convert the Huffman tree into a table of
// Huffman codes
void tree2table_recursive(struct huffcoder * this, struct huffchar * node,
		 unsigned int path, int depth)
{
    if(node->is_compound == 1){
        path = path << 1;
        tree2table_recursive(this, node->u.compound.left, path, depth+1);
        path = path | 1;
        tree2table_recursive(this, node->u.compound.right, path, depth+1);
    }
    else{
        unsigned char index = node->u.c;
        this->codes[index] = (long)path;
        this->code_lengths[index] = depth;
    }
}

// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object
void huffcoder_tree2table(struct huffcoder * this)
{
    unsigned int path = 0;
    int depth = 0;
    if(this->tree != NULL){
        tree2table_recursive(this, this->tree  , path, depth);
    }
}


// print the Huffman codes for each character in order
void huffcoder_print_codes(struct huffcoder * this)
{
  int i, j;
  char buffer[NUM_CHARS];

  for ( i = 0; i < NUM_CHARS; i++ ) {
    // put the code into a string
    for ( j = this->code_lengths[i]-1; j >= 0; j--) {
      buffer[this->code_lengths[i]-1-j] = ((this->codes[i] >> j) & 1) + '0';
    }
    // don't forget to add a zero to end of string
    buffer[this->code_lengths[i]] = '\0';

    // print the code
    printf("char: %d, freq: %d, code: %s\n", i, this->freqs[i], buffer);;
  }
}

void makeCompound(struct huffchar* smallest, struct huffchar* scndSmallest, struct huffchar * compound, int i){
    compound->freq = (smallest->freq + scndSmallest->freq);
    compound->seqno = NUM_CHARS + i;
    compound->is_compound = 1;
    compound->u.compound.left = smallest;
    compound->u.compound.right = scndSmallest;
}


int findMin(struct huffchar* arr[], int counter){
    int smallestIndex = 0;
    int smallestFreq = arr[0]->freq;
    for(int i=0; i<counter; i++){
        if(arr[i]->freq < smallestFreq){
            smallestFreq = arr[i]->freq;
            smallestIndex = i;
        }
        else if(arr[i]->freq == smallestFreq){
            if(arr[i]->seqno < arr[smallestIndex]->seqno){
                smallestIndex = i;
                smallestFreq = arr[i]->freq;
            }
        }
    }
    return smallestIndex;
}




