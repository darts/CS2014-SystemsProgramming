#include <stdio.h> 
#include <stdlib.h> 
#include <stddef.h> 
#include <math.h> 

int main(int argc, char const *argv[])
{
    double itemRead;
    FILE *file;
    file = fopen("input.txt", "r");
    if (file){
        while ((itemRead = getc(file)) != EOF){ //while there are still numbers
                putchar(itemRead); //show the character being read in
        }
    }
}
    