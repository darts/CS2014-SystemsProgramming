#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int BLOOM_SEED1 = 17;
const int BLOOM_SEED2 = 29;
const char * BLOOM_KEYWORD = "bloom";
const char * BITSET_KEYWORD = "bitvector";

//create the bitset scaffold
struct bitset{
    unsigned int *bits;
    int size_in_bits;
    int size_in_words;
    int bits_per_word;
};

//create a new empty bitset
struct bitset * bitset_new(int size){
    struct bitset *result;
    int size_in_words;
    int size_in_bits;
    int bits_per_word = (sizeof(unsigned int) * 8);
    result = malloc(sizeof(struct bitset));
    size_in_words = size / (bits_per_word);
    if(size % bits_per_word != 0)
        size_in_words++;
    result->bits = malloc((sizeof(unsigned int) * size_in_words));
    for(int i = 0; i < size_in_words; i++)
        result->bits[i] = 0;
    result->size_in_bits = size;
    result->size_in_words = size_in_words;
    result->bits_per_word = bits_per_word;
    return result;
}

//Get the number of items capable of being stored in the set
int bitset_size(struct bitset *this){
    if(this != NULL)
        return this->size_in_bits;
    return -1;
}

//Get the number of items currently stored in the set
//Use Brian Kernighan's algorithm because it's super cool and efficient
//The loop runs A times with A being the number of set bits
int bitset_cardinality(struct bitset *this){
    int count = 0;
    if(this != NULL){
        for(int i = 0; i < this->size_in_words; i++){
            unsigned int currentInt = this->bits[i];
            while(currentInt){
                currentInt &= (currentInt-1);
                count++;
            }
        }
    }
    return count;
}

//print the binary value of an unsigned number
//was created for testing(debugging) and could still be useful
void printBin(unsigned num){
    unsigned mask = 2147483648;
    for(int i = 0; i < 32; i++){
        if((num & mask) == 0)
            printf("0");
        else    
            printf("1");
        num<<=1;
    }
    printf("\n");
}

//Check to see if an item is contained in the set
//Returns 1 if the value is in the set, otherwise returns 0
int bitset_lookup(struct bitset *this, int item){
    if(this != NULL){
        int word_num = item / this->bits_per_word;
        int bit_num = item % this->bits_per_word;
        unsigned int word = this->bits[word_num];
        unsigned mask = 1<<bit_num;
        if((mask&word) != 0)
            return 1;
        return 0;
    }
    return 0;
}

//add an item with num 'item' to the set
int bitset_add(struct bitset *this, int item){
    if(this != NULL){
        int word_num = item / this->bits_per_word;
        int bit_num = item % this->bits_per_word;
        unsigned mask = 1<<bit_num;
        this->bits[word_num] = mask | this->bits[word_num];
        return 1;
    }
    return -1;
}



//remove an item with num 'item' form the set
int bitset_remove(struct bitset *this, int item){
     if(this != NULL && item < 0){
        int word_num = item / this->bits_per_word;
        int bit_num = item % this->bits_per_word;
        unsigned int mask = ~(1<<bit_num);
        this->bits[word_num] &= mask;
        return 1;
    }
    return -1;
}

//place the union of sets src1 and src2 into dst set
void bitset_union(struct bitset *dest, struct bitset *src1, struct bitset *src2){
    assert(dest != NULL && src1 != NULL && src2 != NULL);
    assert(dest->bits_per_word == src1->bits_per_word && src1->bits_per_word == src2->bits_per_word);
    assert(dest->size_in_bits == src1->size_in_bits && src1->size_in_bits == src2->size_in_bits);
    for(int i = 0; i < dest->size_in_words; i++){
        dest->bits[i] = src1->bits[i] | src2->bits[i];
    }
}

//place int intersection of sets src1 and src2 into dst set
void bitset_intersection(struct bitset *dest, struct bitset *src1, struct bitset *src2){
    assert(dest != NULL && src1 != NULL && src2 != NULL);
    assert(dest->bits_per_word == src1->bits_per_word && src1->bits_per_word == src2->bits_per_word);
    assert(dest->size_in_bits == src1->size_in_bits && src1->size_in_bits == src2->size_in_bits);
    for(int i = 0; i < dest->size_in_words; i++){
        dest->bits[i] = src1->bits[i] & src2->bits[i];
    }
}

//print the contents of a bitset ------------- this is a testing function
void bitset_print(struct bitset *this){
    int size = bitset_size(this);
    for(int i = 0; i < size; i++){
        if(bitset_lookup(this, i) == 1){
            printf("%d ", i);
        }
    }
    printf("\n");
}

//add the characters from a string to the bitset ------------- this is a testing function
//charArray must be null terminated
void add_chars_to_set(struct bitset *this, char *charArray){
    for(int i = 0; charArray[i] != 0; i++){
        unsigned tmp = charArray[i];
        bitset_add(this, tmp);
    }
}

int hash_string(char * string, int seed, int range){
    int i;
    int hash = 0;

  // simple loop for mixing the input string
  for (i = 0; string[i] != '\0'; i++) {
    hash = hash * seed + string[i];
  }
  // check for unlikely case that hash is negative
  if (hash < 0) {
    hash = -hash;
  }
  // bring the hash within the range 0..range-1
  hash = hash % range;
  
  return hash;
}

struct bloom{
    struct bitset *filter;
};

// create a new, empty Bloom filter of 'size' items
struct bloom * bloom_new(int size){
    struct bloom *theBloom;
    theBloom->filter = bitset_new(size);
    return theBloom;
};

// check to see if a string is in the set
int bloom_lookup(struct bloom * this, char * item){
    int hash1 = hash_string(item, BLOOM_SEED1, this->filter->size_in_bits);
    if(bitset_lookup(this->filter, hash1) == 0)
        return 0;
    int hash2 = hash_string(item, BLOOM_SEED2, this->filter->size_in_bits);
    if(bitset_lookup(this->filter, hash2) == 0)
        return 0;
    return 1;
};

// add a string to the set
// has no effect if the item is already in the set
void bloom_add(struct bloom * this, char * item){
    int hash1 = hash_string(item, BLOOM_SEED1, this->filter->size_in_bits);
    int hash2 = hash_string(item, BLOOM_SEED2, this->filter->size_in_bits);
    bitset_add(this->filter, hash1);
    bitset_add(this->filter, hash2);
};

// note that you cannot safely remove items from a Bloom filter

// place the union of src1 and src2 into dest
void bloom_union(struct bloom * dest, struct bloom * src1, struct bloom * src2){
    bitset_union(dest->filter, src1->filter, src2->filter);
};

// place the intersection of src1 and src2 into dest
void bloom_intersect(struct bloom * dest, struct bloom * src1, struct bloom * src2){
    bitset_intersection(dest->filter, src1->filter, src2->filter);
}

//Small routine to test a bitset  ------------- this is a testing function
void myTest(){
    struct bitset *a = bitset_new(256);
    struct bitset *b = bitset_new(256);
    struct bitset *c = bitset_new(256);
    char *string1 = "What can you hear?";
    char *string2 = "Nothing but the rain?";

    add_chars_to_set(a, string1);
    add_chars_to_set(b, string2);

    //print the contents of the sets
    bitset_print(a);
    bitset_print(b);

    //compute and print the union of sets
    bitset_union(c, a, b);
    bitset_print(c);

    //compute and print the intersection of sets
    bitset_intersection(c, a, b);
    bitset_print(c);
}

void errExit(){
    printf("Incorrect number of parameters supplied.\n");
    printf("Usage: 'bloom' <file1> <file2> OR 'bitvector' <file1>\n");
    exit(1);
}

void errNoFile(char *filename){
    printf("Could not find file with name: %s",filename);
    exit(1);
}



int main(int argc, char ** argv){
    char * runType;
    char * file1;
    char * file2;
    int isBloom;

    if ( argc == 2 ) { //2 args -> bitvector
        runType = argv[1];
        file1 = argv[2];
        if(strcmp(runType, BITSET_KEYWORD) != 0)
            errExit();
        printf("Running as bitvector.\n");
        isBloom = 0;
    }else if ( argc == 3 ) { //3 args -> bloom filter
        runType = argv[1];
        file1 = argv[2];
        file2 = argv[3];
        if(strcmp(runType, BLOOM_KEYWORD) != 0)
            errExit();
        printf("Running as Bloom Filter.\n");
        isBloom = 1;
    }else { //num of args != 2|3 -> incorrect usage
        errExit();
    }

    FILE *fileA = fopen(file1, "r");
    FILE *fileB;
    if(isBloom == 1)
        fileB = fopen(file2, "r");
    
    





}