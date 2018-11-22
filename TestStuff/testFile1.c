#include <stdio.h>

void testThings1(char * w, char * r){
    printf("%d\n", !(*w & 1));
    printf("%d\n", !(*r & 1));
}

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

void testThings2(){
    printBin((int)"x");
    // printBin((int)"‡");
    printf("\n");
    printBin((int)"ޜ");
    printBin((int)"=");

}

int main(int argc, char ** argv){
    // testThings1("w", "r");
    testThings2();
}