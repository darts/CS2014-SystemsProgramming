#include <stdio.h>

void testThings(char * w, char * r){
    printf("%d\n", !(*w & 1));
    printf("%d\n", !(*r & 1));
}

int main(int argc, char ** argv){
    testThings("w", "r");
}