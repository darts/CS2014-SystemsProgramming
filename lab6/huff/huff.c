// code for a huffman coder

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "huff.h"

// create a new huffcoder structure
struct huffcoder *huffcoder_new()
{
  struct huffcoder *coder = malloc(sizeof(struct huffcoder)); //get some space for the huffcoder
  coder->tree = malloc(sizeof(struct huffchar *));
  return coder;
}

// count the frequency of characters in a file; set chars with zero
// frequency to one
void huffcoder_count(struct huffcoder *this, char *filename)
{
  FILE *file;                  //new file
  file = fopen(filename, "r"); //open the file
  char itemRead;
  while ((itemRead = getc(file)) != EOF) //read every character
  {
    this->freqs[(int)itemRead]++; //increment counter
  }

  for (int i = 0; i < NUM_CHARS; i++) //if we havent found any characters, pretend we found 1
  {
    if (this->freqs[i] == 0)
      this->freqs[i]++;
  }
}

//Get the smallest two characters in the set
int get_smallest(int freq[])
{
  int locOfSmallest; //this is the number to void at the end
  int smallest = -1; //smallest number
  for (int i = 0; i < NUM_CHARS; i++)//go through the array from the start
  {
    if (freq[i] != -1)//number is not void
    {
      if (smallest == -1)//first iteration num
      {
        smallest = freq[i];
        if(smallest == 1){//no numbers smaller than 1 -> this is smallest 
          freq[i] = -1; //set loc to void
          return smallest;
        }
      }
      else//this is not the first number
      {
        if(smallest > freq[i]){//if the new number is less than the smallest number 
          smallest = freq[i]; //set the smallest to the new smallest
          locOfSmallest = i; //update the location of the smallest number
        }
        if(smallest == 1){//is the smallest possible number
          freq[locOfSmallest] = -1; //set location of smallest to void
          return smallest;
        }
      }
    }
  }
  freq[locOfSmallest] = -1; //void the smallest number
  return smallest;
}

// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes
void huffcoder_build_tree(struct huffcoder *this)
{
}

// recursive function to convert the Huffman tree into a table of
// Huffman codes
void tree2table_recursive(struct huffcoder *this, struct huffchar *node,
                          int *path, int depth)
{
}

// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object
void huffcoder_tree2table(struct huffcoder *this)
{
}

// print the Huffman codes for each character in order
void huffcoder_print_codes(struct huffcoder *this)
{
  int i, j;
  char buffer[NUM_CHARS];

  for (i = 0; i < NUM_CHARS; i++)
  {
    // put the code into a string
    for (j = this->code_lengths[i] - 1; j >= 0; j--)
    {
      buffer[j] = ((this->codes[i] >> j) & 1) + '0';
    }
    // don't forget to add a zero to end of string
    buffer[this->code_lengths[i]] = '\0';

    // print the code
    printf("char: %d, freq: %d, code: %s\n", i, this->freqs[i], buffer);
    ;
  }
}
