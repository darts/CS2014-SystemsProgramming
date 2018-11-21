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
void printHuff(struct huffchar* node){
  fprintf(stderr, "freq: %d isCompound: %d seqNum:%d\n", node->freq,node->is_compound,node->seqno);
}
//Move the smallest two characters in the set to the front
void sort_smallest(struct huffchar *huffArr[])
{
  struct huffchar *tmpHuff = malloc(sizeof(struct huffchar));
  int isFirstRun = 2;
  for(int i = 0; i < NUM_CHARS; i++){
    
    if(huffArr[i] != NULL){
      if(isFirstRun-- == 2){
        *tmpHuff = *huffArr[i];
        *huffArr[i] = *huffArr[0];
        *huffArr[0] = *tmpHuff;
      }else if(isFirstRun == 1){
        *tmpHuff = *huffArr[i];
        *huffArr[i] = *huffArr[1];
        *huffArr[1] = *tmpHuff;
      }else{
        if((huffArr[i]->freq < huffArr[0]->freq) || (huffArr[i]->freq == huffArr[0]->freq && huffArr[i]->seqno < huffArr[0]->seqno)){
          *tmpHuff = *huffArr[i];
          *huffArr[i] = *huffArr[1];
          *huffArr[1] = *huffArr[0];
          *huffArr[0] = *tmpHuff;
        }else if((huffArr[i]->freq < huffArr[1]->freq) || (huffArr[i]->freq == huffArr[1]->freq && huffArr[i]->seqno < huffArr[1]->seqno)){
          *tmpHuff = *huffArr[i];
          *huffArr[i] = *huffArr[1];
          *huffArr[1] = *tmpHuff;
        } 
      }
    }
  }
}

struct huffchar *new_Huff(int freq, int seqNum, char theChar)
{
  struct huffchar *theHuff = malloc(sizeof(struct huffchar));
  theHuff->freq = freq;
  theHuff->is_compound = 0;
  theHuff->seqno = seqNum;
  theHuff->u.c = theChar;
  return theHuff;
}

struct huffchar *new_HuffCompound(struct huffchar *left, struct huffchar *right, int seqNum)
{
  struct huffchar *theHuff = malloc(sizeof(struct huffchar));
  theHuff->freq = left->freq + right->freq;
  // printf("left:%d right:%d total:%d", left->freq,right->freq,theHuff->freq);
  theHuff->is_compound = 1;
  theHuff->seqno = seqNum;
  theHuff->u.compound.left = left;
  theHuff->u.compound.right = right;
  return theHuff;
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
    printHuff(huffArr[0]);
    sort_smallest(huffArr);
    printHuff(huffArr[0]);
    huffArr[0] = new_HuffCompound(huffArr[0], huffArr[1], NUM_CHARS + i);
    struct huffchar theChar = *huffArr[1];
    huffArr[1] = huffArr[NUM_CHARS - i];
    // printHuff(huffArr[0]);
  }
  this->tree = huffArr[0];
}

// recursive function to convert the Huffman tree into a table of
// Huffman codes
void tree2table_recursive(struct huffcoder *this, struct huffchar *node,
                          int *path, int depth)
{
  fprintf(stderr,"this is recursive.");
  fprintf(stderr, "%d\n", node->is_compound);
  if(node->is_compound == 1){
    *path = *path << 1;
    tree2table_recursive(this, node->u.compound.left, path, ++depth);
    *path = *path | 1;
    tree2table_recursive(this,node->u.compound.right, path, ++depth);
  }else{
    this->code_lengths[node->u.c] = depth;
    this->codes[node->u.c] = (long)path;
    // fprintf(stderr, "%d  ", depth);
  }
}

// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object
void huffcoder_tree2table(struct huffcoder *this)
{
  tree2table_recursive(this, this->tree, 0, 0);
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
