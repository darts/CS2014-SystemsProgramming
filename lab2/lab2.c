#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

struct listNode //a node on the list
{
    double data;           //data in the node
    struct listNode *next; //pointer to next node
};

struct list //pointer to a node
{
    struct listNode *head; //create a pointer called head to point at first node in list
};

struct list *new_empty_list() //creates a pointer to a list
{
    struct list *result;                  //creates a pointer to a list
    result = malloc(sizeof(struct list)); //allocate memory to the list
    result->head = NULL;                  //point list's pointer to NULL
    return result;                        // return the pointer to the list
};

void add_to_front_of_list(struct list *this, int data)
{
    struct listNode *node = malloc(sizeof(struct listNode)); // allocate memory to new listnode
    node->data = data;                                       //set the data in the new node to be the data that was passed
    node->next = this->head;                                 // set the pointer in the new node to be the current pointer
    this->head = node;                                       // set the listNode pointer to be the new node
}

double getPrev(struct list *list)
{
    double numRet = (list->head)->data;      //get the data from the node
    struct listNode *tmpHead = (list->head); //get a temporary pointer to the node
    list->head = (list->head)->next; //update the pointer in head to point to the next node
    free(tmpHead);  //free the old node
    return numRet;  //return the data
}

void multiply(struct list *list)
{
    double numA = getPrev(list); //get the previous number
    double numB = getPrev(list); //^^

    numA = numA * numB; //multiply the two
    add_to_front_of_list(list, numA); //add to the front of the list
}

void sum(struct list *list)
{
    double numA = getPrev(list); //get the previous number
    double numB = getPrev(list); //^^

    numA = numA + numB; //sum the two
    add_to_front_of_list(list, numA); //add to the front of the list
}

void subtract(struct list *list)
{
    double numA = getPrev(list); //get previous number
    double numB = getPrev(list); //^^

    numA = numB - numA; //get the difference
    add_to_front_of_list(list, numA); //add to the front of the list
}

void divide(struct list *list)
{
    double numA = getPrev(list); //get previous number 
    double numB = getPrev(list); //^^

    numA = numB / numA; //divide the numbers
    add_to_front_of_list(list, numA); //add to the front of the list
}

void power(struct list *list)
{
    double numA = getPrev(list);//get the previous number 
    double numB = getPrev(list);//^^

    numA = pow(numA, numB); //raise to the power of....
    add_to_front_of_list(list, numA); //add to the front of the list
}

double normalise(double prev, double current) //for multiple-digit numbers
{
    prev = prev * 10; //offset by x10
    prev += current; //add to prev
    return prev; //return 
}

int main(int argc, char const *argv[])
{
    double itemRead;
    FILE *file;
    file = fopen("input.txt", "r");
    if (file)
    {
        double prev = -1; //no prev number
        struct list *numList = new_empty_list(); //new list
        while ((itemRead = getc(file)) != EOF) //while there are still numbers
        {   
            putchar(itemRead); //show the character being read in
            if (itemRead == 'X')
            {
                multiply(numList);
            }
            else if (itemRead == '-')
            {
                subtract(numList);
            }
            else if (itemRead == '/')
            {
                divide(numList);
            }
            else if (itemRead == '^')
            {
                power(numList);
            }
            else if (itemRead == '+')
            {
                sum(numList);
            }
            else
            {
                if (itemRead == ' ')
                {
                    if(prev != -1){ //has previous number
                        add_to_front_of_list(numList, prev);
                        prev = -1; //reset to false
                    }
                }
                else
                {
                    double num = itemRead - '0'; //convert to double
                    if(prev == -1)//if this is the first number, set prev to 0
                        prev = 0;
                    prev = normalise(prev, num);
                }
            }
        }
        fclose(file);
        printf("\nThe result is:%f \n", (numList->head)->data);
    }
    return 0;
}