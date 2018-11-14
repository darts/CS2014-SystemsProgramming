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

//Move the smallest two characters in the set to the front
void sort_smallest(struct huffchar *huffArr[])
{
  struct huffchar *smallest = NULL;
  struct huffchar *secondSmallest = NULL;
  int smallestLoc = -1;
  int secondSmallestLoc = -1;
  for (int i = 0; i < NUM_CHARS; i++) //go through the array from the start
  {
    if (huffArr[i] != NULL) //number is not void
    {
      if (smallest == NULL)
      {
        smallest = huffArr[i];
        smallestLoc = i;
      }
      else if (huffArr[i]->freq < smallest->freq && huffArr[i]->seqno < smallest->seqno)
      {
        secondSmallest = smallest;
        secondSmallestLoc = smallestLoc;
        smallest = huffArr[i];
        smallestLoc = i;
      }else if((huffArr[i]->freq < secondSmallest->freq && huffArr[i]->seqno < secondSmallest->seqno) || secondSmallest == NULL){
        secondSmallest = huffArr[i];
        secondSmallestLoc = i;
      }
    }
  }

}

struct huffchar *new_Huff(int freq, int isCompound, int seqNum)
{
  struct huffchar *theChar = malloc(sizeof(struct huffchar));
  theChar->freq = freq;
  theChar->is_compound = isCompound;
  theChar->seqno = seqNum;
  return theChar;
}

// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes
void huffcoder_build_tree(struct huffcoder *this)
{
  struct huffchar *huffArr[NUM_CHARS];
  for (int i = 0; i < NUM_CHARS; i++)
  {
    huffArr[i] = new_Huff(this->freqs[i], 0, i);
  }
  for (int i = 0; i < NUM_CHARS - 1; i++)
  {
  }
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
