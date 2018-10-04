//Infix-Notated inputs must be wrapped in brackets eg 3 + 5 -> ( 3 + 5 )
//Must have space between each number/operator/bracket

#include <stdio.h> 
#include <stdlib.h> 
#include <stddef.h> 
#include <math.h> 
#include <string.h>

struct listNode {//a node on the list
    double data; //data in the node
    struct listNode * next; //pointer to next node
}; 

struct list {//pointer to a node 
    struct listNode * head; //create a pointer called head to point at first node in list
}; 

struct list * newEmptyList() {//creates a pointer to a list
    struct list * result; //creates a pointer to a list
    result = malloc(sizeof(struct list)); //allocate memory to the list
    result -> head = NULL; //point list's pointer to NULL
    return result;                        // return the pointer to the list
};

void addToFrontOfList(struct list *this, double data) {//Add to the front of the list/top of the stack
    struct listNode *node = malloc(sizeof(struct listNode)); // allocate memory to new listnode
    node->data = data;                                       //set the data in the new node to be the data that was passed
    node->next = this->head;                                 // set the pointer in the new node to be the current pointer
    this->head = node;                                       // set the listNode pointer to be the new node
}

void addToEndOfList(struct list *list, double data){ //Add to the end of the list/bottom of the stack
    struct listNode *node = malloc(sizeof(struct listNode)); // allocate memory to new listnode
    struct listNode *tmpHead = list->head;                   //create new temporary node
    if (list->head != NULL) {                                //if the list size is not 0
        while (tmpHead->next != NULL){ //while not at the end of the list
            tmpHead = tmpHead->next; //Iterate through the list
        }
        tmpHead->next = node; //point to new node
    }else{
        list->head = node; //point to new node
    }
    node->data = data; //add new data to the node
    node->next = NULL; //give the node a new pointer
}

double getPrev(struct list *list){ //get the previous node and remove it
    double numRet = (list->head)->data;      //get the data from the node
    struct listNode *tmpHead = (list->head); //get a temporary pointer to the node
    list->head = (list->head)->next;         //update the pointer in head to point to the next node
    free(tmpHead);                           //free the old node
    return numRet;                           //return the data
}

void multiply(struct list *list){
    double numA = getPrev(list); //get the previous number
    double numB = getPrev(list); //^^
    numA = numA * numB;           //multiply the two
    addToFrontOfList(list, numA); //add to the front of the list
}

void sum(struct list *list){
    double numA = getPrev(list); //get the previous number
    double numB = getPrev(list); //^^
    numA = numA + numB;           //sum the two
    addToFrontOfList(list, numA); //add to the front of the list
}

void subtract(struct list *list){
    double numA = getPrev(list); //get previous number
    double numB = getPrev(list); //^^
    numA = numB - numA;           //get the difference
    addToFrontOfList(list, numA); //add to the front of the list
}

void divide(struct list *list){
    double numA = getPrev(list); //get previous number
    double numB = getPrev(list); //^^
    numA = numB / numA;           //divide the numbers
    addToFrontOfList(list, numA); //add to the front of the list
}

void power(struct list *list){
    double numA = getPrev(list); //get the previous number
    double numB = getPrev(list); //^^
    numA = pow(numB, numA);       //raise to the power of....
    addToFrontOfList(list, numA); //add to the front of the list
}

void normalise(double *prev, double current){ //for multiple-digit numbers
    *prev = *prev * 10; //offset by x10
    *prev += current;   //add to prev
}

void updatePostFix(double itemRead, struct list *numList, double *prev){
    if (itemRead == 'X'){
        multiply(numList);
    }else if (itemRead == '-'){
        subtract(numList);
    }else if (itemRead == '/'){
        divide(numList);
    }else if (itemRead == '^'){
        power(numList);
    }else if (itemRead == '+'){
        sum(numList);
    }else{
        if (itemRead == ' '){
            if (*prev != -1){ //has previous number
                addToFrontOfList(numList, *prev);
                *prev = -1; //reset to false
            }
        }else{
            double num = itemRead - '0'; //convert to double
            if (*prev == -1)             //if this is the first number, set prev to 0
                *prev = 0;
            normalise(prev, num); //convert to a proper int ie 1 5 -> 15
        }
    }
}

void manageInFixStack(double itemRead, double *prev, struct list *opList, struct list *rpnList){
    if (itemRead == '('){
        addToFrontOfList(opList, itemRead);
    }else if (itemRead == ')'){
        while (opList->head->data != '('){
            addToEndOfList(rpnList, getPrev(opList));
            addToEndOfList(rpnList, ' ');
        }
        getPrev(opList);
    }else if (itemRead == '^'){ //check precedence
        while (opList->head != NULL && opList->head->data == '^'){ //check precedence
            addToEndOfList(rpnList, getPrev(opList));
            addToEndOfList(rpnList, ' ');
        }
        addToFrontOfList(opList, itemRead);
    }else if (itemRead == 'X' || itemRead == '/'){ //check precedence
        while (opList->head != NULL && opList->head->data == '^') //check precedence
        {
            addToEndOfList(rpnList, getPrev(opList));
            addToEndOfList(rpnList, ' ');
        }
        addToFrontOfList(opList, itemRead);
    }else if (itemRead == '-' || itemRead == '+'){ //check precedence
        while (opList->head != NULL && (opList->head->data == '^' || opList->head->data == 'X' || opList->head->data == '/')){ //check precedence
            addToEndOfList(rpnList, getPrev(opList));
            addToEndOfList(rpnList, ' ');
        }
        addToFrontOfList(opList, itemRead);
    }else{
        if (itemRead == ' '){
            if (*prev != -1){ //has previous number
                addToEndOfList(rpnList, ' ');
                *prev = -1; //reset to false
            }
        }else{
            double num = itemRead - '0'; //convert to double
            addToEndOfList(rpnList, itemRead);
            if (*prev == -1) //if this is the first number, set prev to 0
                *prev = 0;
        }
    }
}

//evaluate the input (RPN / infix)
void evalInput(int *firstCharIsBracket, double itemRead, double *prev,
               struct list *numList, struct list *opList, struct list *rpnList){
    if (*firstCharIsBracket == -1){ //first char
        if (itemRead == 'i'){
            *firstCharIsBracket = 1; //use infix
        }else{
            *firstCharIsBracket = 0;
        }
    }else if (*firstCharIsBracket == 0){ //use RPN
        updatePostFix(itemRead, numList, prev);
    }else{
        manageInFixStack(itemRead, prev, opList, rpnList); //use infix
    }
}

//infix has been converted to RPN, now calculate
void postfixOnStack(struct list *rpnList, struct list *numList)
{
    double prevNum = -1;
    double *prev = &prevNum;
    while (rpnList->head != NULL){
        updatePostFix(rpnList->head->data, numList, prev); //use RPN calc
        getPrev(rpnList);                                  //remove whitespace
    }
}

void pushInfixStack(struct list *rpnList, struct list *opList){
    while(opList->head != NULL)
        addToEndOfList(rpnList, getPrev(opList));
}

int main(int argc, char ** argv){
    char * filename;

    if ( argc == 1 ) {
        printf("Error: No input filename provided\n");
        printf("Usage: %s <input filename>\n", argv[0]);
        exit(1);
  }else if ( argc > 2 ) {
        printf("Error: Too many command line parameters\n");
        printf("Usage: %s <input filename>\n", argv[0]);
        exit(1);
  }else {
    filename = argv[1];
  }

    double itemRead;
    FILE *outFile;
    FILE *file;
    file = fopen(filename, "r");
    char *newFileName = strcat(filename, ".results");
    outFile = fopen(newFileName, "w");
    if(outFile != NULL){
      //  printf("no file ;-;");
    }
    if (file){
        int firstCharIsBracket = -1;
        int *firstCharIsBracketBool = &firstCharIsBracket;
        int isFirstChar = 1;
        double prevNum = -1;
        double *prev = &prevNum;               //no prev number
        struct list *numList = newEmptyList(); //new list
        struct list *opList = newEmptyList();  //new tmp list for infix -> RPN
        struct list *rpnList = newEmptyList(); //output list
        while ((itemRead = getc(file)) != EOF){ //while there are still numbers
            if (itemRead != '\n'){
                putc(itemRead, outFile);
                evalInput(firstCharIsBracketBool, itemRead, prev, numList, opList, rpnList);
                if(isFirstChar == 1){
                    int n = 0;
                    if(firstCharIsBracket == 1){//how many chars to print
                        n = 5;
                    }else{
                        n = 6;
                    }
                    for(int i = 0; i < n; i++){//read and print the chars
                        double read = getc(file);
                        putc(read, outFile);
                    }
                    isFirstChar = 0;
                }
            }else{
                fprintf(outFile, "\n");
                if (firstCharIsBracket == 1){
                    firstCharIsBracket = 0;
                    pushInfixStack(rpnList, opList);
                    postfixOnStack(rpnList, numList);
                }
                fprintf(outFile, "%f\n", (numList->head)->data);
                isFirstChar = 1;
                prevNum = -1;
                firstCharIsBracket = -1;
                numList = newEmptyList();
                opList = newEmptyList();
                rpnList = newEmptyList();
            }
        }
        if (firstCharIsBracket == 1){
            firstCharIsBracket = 0;
            pushInfixStack(rpnList, opList);
            postfixOnStack(rpnList, numList);
        }
        fclose(file);
        fprintf(outFile, "\n%f", (numList->head)->data);
    }
    return 0;
}