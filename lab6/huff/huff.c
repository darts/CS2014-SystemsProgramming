// code for a huffman coder


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "huff.h"


// create a new huffcoder structure
struct huffcoder *  huffcoder_new()
{
  struct huffcoder *theCoder = malloc(sizeof(struct huffcoder));
  return theCoder;
}

// count the frequency of characters in a file; set chars with zero
// frequency to one
void huffcoder_count(struct huffcoder * this, char * filename)
{
  unsigned char c;  // we need the character to be
                    // unsigned to use it as an index
  FILE * file;

  file = fopen(filename, "r");
  assert( file != NULL );
  c = fgetc(file);	// attempt to read a byte
  while(!feof(file)) {
    this->freqs[c]++;
    c = fgetc(file);
  }
  fclose(file);
}

struct huffchar * huffchar_new(){
  struct huffchar *theChar = malloc(sizeof(struct huffchar));
  return theChar;
}

void printHuff(struct huffchar *theChar){
  fprintf(stderr, "%d %d %d\n", theChar->freq,theChar->is_compound, theChar->seqno);
}

void swapChars(struct huffchar *charArr[], int locA, int locB){
  struct huffchar *tmpA = huffchar_new();
  tmpA = charArr[locA];
  charArr[locA] = charArr[locB];
  charArr[locB] = tmpA;  
}

void sortOne(struct huffchar *charArr[], int start, int max){
  for(int i = start + 1; i < max; i++){
    if((charArr[start]->freq > charArr[i]->freq) || 
    (charArr[start]->freq == charArr[i]->freq && charArr[start]->seqno > charArr[i]->seqno)){
      swapChars(charArr, i, start);
    }
  }
}

void sortList(struct huffchar *charArr[], int max){
  sortOne(charArr, 0, max);
  sortOne(charArr, 1, max);
}

struct huffchar * combine_huffs(struct huffchar *huffL, struct huffchar *huffR, int seqNum){
  struct huffchar *theChar = huffchar_new();
  theChar->freq = (huffL->freq + huffR->freq);
  theChar->seqno = seqNum;
  theChar->is_compound = 1;
  theChar->u.compound.left = huffL;
  theChar->u.compound.right = huffR;
  return theChar;
}

// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes
void huffcoder_build_tree(struct huffcoder * this)
{
  struct huffchar *charArr[NUM_CHARS];
  for(int i = 0; i < NUM_CHARS; i++){
    struct huffchar *theChar = huffchar_new();
    theChar->u.c = i;
    theChar->freq = this->freqs[i];
    if(theChar->freq == 0)
      theChar->freq = 1;
    theChar->is_compound = 0;
    theChar->seqno = i;
    charArr[i] = theChar;
    // fprintf(stderr, "char:%d  freq:%d\n", theChar->u.c, theChar->freq);
  }
  for(int i = 0; i < NUM_CHARS; i++){
    // fprintf(stderr, "1   freqA:%d  freqB:%d\n",charArr[0]->freq, charArr[1]->freq);
    sortList(charArr, NUM_CHARS - i);
    fprintf(stderr, "char:%d\n", charArr[0]->u.c);
    // fprintf(stderr, "2   freqA:%d  freqB:%d\n",charArr[0]->freq, charArr[1]->freq);
    charArr[0] = combine_huffs(charArr[0], charArr[1], NUM_CHARS + i);
    // fprintf(stderr, "2   A:%d, B:%d, AB:%d\n", charArr[0]->u.compound.left->freq, charArr[0]->u.compound.right->freq, charArr[0]->freq );
    // printHuff(charArr[0]);
    // fprintf(stderr, "3   freqA:%d  freqB:%d\n",charArr[0]->freq, charArr[1]->freq);
    // fprintf(stderr, "3   A:%d, B:%d, AB:%d\n", charArr[0]->u.compound.left->freq, charArr[0]->u.compound.right->freq, charArr[0]->freq );
    swapChars(charArr, 1, (NUM_CHARS - 1)- i);
    // fprintf(stderr, "4   A:%d, B:%d, AB:%d\n", charArr[0]->u.compound.left->freq, charArr[0]->u.compound.right->freq, charArr[0]->freq );
    // fprintf(stderr, "4   freqA:%d  freqB:%d\n",charArr[0]->freq, charArr[1]->freq);
  }
  this->tree = charArr[0];
}

void printBin(unsigned num, int strtAt){
    unsigned mask = 2147483648;
    for(int i = 0; i < 32; i++){
      if(i > strtAt){
        if((num & mask) == 0)
            fprintf(stderr,"0");
        else    
            fprintf(stderr, "1");
      }
      num<<=1;
    }
    fprintf(stderr,"\n");
}

// recursive function to convert the Huffman tree into a table of
// Huffman codes
void tree2table_recursive(struct huffcoder * this, struct huffchar * node,
		 int path, int depth)
{
  // fprintf(stderr, "this is recursion");
  // fprintf(stderr, "%d\n", node->freq);
  if(node->is_compound == 1){
    path <<= 1;
    tree2table_recursive(this, node->u.compound.left, path, depth+1);
    path |= 1;
    // fprintf(stderr,"path: %d\n", path);
    tree2table_recursive(this, node->u.compound.right, path, depth+1);
  }else{
    fprintf(stderr, "char:%d path:%d depth:%d\n", node->u.c, path,depth);
    printBin(path,32-depth);
    this->codes[node->u.c] = (long)path;
    this->code_lengths[node->u.c] = depth;
  }
}

// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object
void huffcoder_tree2table(struct huffcoder * this)
{
  tree2table_recursive(this, this->tree, 0, 0);
}


// print the Huffman codes for each character in order
void huffcoder_print_codes(struct huffcoder * this)
{
  int i, j;
  char buffer[NUM_CHARS];

  for ( i = 0; i < NUM_CHARS; i++ ) {
    // put the code into a string
    for ( j = this->code_lengths[i]-1; j >= 0; j--) {
      buffer[j] = ((this->codes[i] >> j) & 1) + '0';
    }
    // don't forget to add a zero to end of string
    buffer[this->code_lengths[i]] = '\0';

    // print the code
    printf("char: %d, freq: %d, code: %s **depth%d\n", i, this->freqs[i], buffer, this->code_lengths[i]);
  }
}
