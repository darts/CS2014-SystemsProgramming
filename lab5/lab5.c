#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

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
    int bits_per_word = sizeof(unsigned) * 8;
    result = malloc(sizeof(struct bitset));
    size_in_words = size / (bits_per_word);
    if(size % bits_per_word != 0)
        size_in_words++;
    result->bits = malloc(sizeof(unsigned) * size_in_words);
    for(int i = 0; i < size_in_words; i++)
        result->bits[i] = 0;
    result->size_in_bits = size_in_bits;
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
            unsigned currentInt = this->bits[i];
            while(currentInt){
                currentInt &= (currentInt-1);
                count++;
            }
        }
    }
    return count;
}

//Check to see if an item is contained in the set
//Returns 1 if the value is in the set, otherwise returns 0
int bitset_lookup(struct bitset *this, int item){
    if(this != NULL && item < 0){
        int word_num = item / this->bits_per_word;
        int bit_num = item % this->bits_per_word;
        unsigned word = this->bits[word_num];
        unsigned mask = 1<<bit_num;
        return word & mask;
    }
    return 0;
}

//add an item with num 'item' to the set
int bitset_add(struct bitset *this, int item){
    if(this != NULL && item < 0){
        int word_num = item / this->bits_per_word;
        int bit_num = item % this->bits_per_word;
        unsigned mask = 1<<bit_num;
        this->bits[word_num] |= mask;
        return 1;
    }
    return -1;
}

//remove an item with num 'item' form the set
int bitset_remove(struct bitset *this, int item){
     if(this != NULL && item < 0){
        int word_num = item / this->bits_per_word;
        int bit_num = item % this->bits_per_word;
        unsigned mask = ~(1<<bit_num);
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

//print the contents of a bitset
void bitset_print(struct bitset *this){
    int size = bitset_size(this);
    for(int i = 0; i < size; i++){
        if(bitset_lookup(this, i) == 1)
            printf("%d ", i);
    }
    printf("\n");
}
