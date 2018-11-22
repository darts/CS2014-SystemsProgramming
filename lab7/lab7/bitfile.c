// C code file for  a file ADT where we can read a single bit at a
// time, or write a single bit at a time

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "bitfile.h"
#define BIT_MAX 7
#define BIT_MIN 0
// open a bit file in "r" (read) mode or "w" (write) mode
//readmode 1=read, 0=write 
struct bitfile * bitfile_open(char * filename, char * mode)
{
    struct bitfile * theFile = malloc(sizeof(struct bitfile));
    theFile->file = fopen(filename, mode);
    theFile->is_read_mode = (!(*mode & 1));
    theFile->index = BIT_MIN;
    return theFile;
}
// void printBin(unsigned num){
//     unsigned mask = 2147483648;
//     for(int i = 0; i < 32; i++){
//         if((num & mask) == 0)
//             fprintf(stderr,"0");
//         else    
//             fprintf(stderr,"1");
//         num<<=1;
//     }
//     fprintf(stderr,"\n");
// }
// write a bit to a file; the file must have been opened in write mode
void bitfile_write_bit(struct bitfile * this, int bit)
{
    // printBin(this->buffer);
    // printBin(bit);
    // if(!this->is_read_mode){
    //     this->buffer |= (bit << this->index++);
    //     if(this->index > BIT_MAX){
    //         this->index = BIT_MIN;
    //         fputc(this->buffer, this->file);
    //         this->buffer = 0;
    //     }
    // }
    // printBin(this->buffer);
    // printBin(bit);
    if(!this->is_read_mode){
        this->buffer |= (bit << this->index);
        this->index--;
        if(this->index < BIT_MIN){
            this->index = BIT_MAX;
            fputc(this->buffer, this->file);
            this->buffer = 0;
        }
    }
    // printBin(this->buffer);
}

// read a bit from a file; the file must have been opened in read mode
int bitfile_read_bit(struct bitfile * this)
{
    if(this->is_read_mode){
        if(this->index < BIT_MIN){
            this->buffer = 0;
            this->buffer = fgetc(this->file);
            this->index = BIT_MAX;
        }
        return (this->buffer & (1 << this->index--));
    }
    return -1;
}

// close a bitfile; flush any partially-filled buffer if file is open
// in write mode
void bitfile_close(struct bitfile * this) {
    if(!this->is_read_mode && this->index != BIT_MAX)
        fputc(this->buffer, this->file);
    fclose(this->file);
}

// check for end of file
int bitfile_end_of_file(struct bitfile * this)
{
    if(this->is_read_mode && (int)this->buffer == EOF)
        return 1;
    return 0;
}
