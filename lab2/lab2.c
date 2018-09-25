#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

struct listNode //a node on the list
{
    double data;              //data in the node
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

int getPrev(struct list *list){
    int numRet = (list->head)->data;
    struct listNode *head = list->head;
    list->head = (list->head)->next;
    free(head);
    return numRet;
}

void multiply(struct list *list){
    double numA = getPrev(list);
    double numB = getPrev(list);

    numA = numA * numB;
    add_to_front_of_list(list, numA);
}

void sum(struct list *list){
    double numA = getPrev(list);
    double numB = getPrev(list);

    numA = numA * numB;
    add_to_front_of_list(list, numA);
}

void subtract(struct list *list){
    double numA = getPrev(list);
    double numB = getPrev(list);

    numA = numB - numA;
    add_to_front_of_list(list, numA);
}

void div(struct list *list){
    double numA = getPrev(list);
    double numB = getPrev(list);

    numA = numB / numA;
    add_to_front_of_list(list, numA);
}

void pow(struct list *list){
    double numA = getPrev(list);
    double numB = getPrev(list);

    numA = numB ^ numA;
    add_to_front_of_list(list, numA);
}

int main(int argc, char const *argv[])
{
    //     //struct list *newList = new_empty_list();
    //     //add_to_front_of_list(newList, 12);

    double itemRead;
    FILE *file;
    file = fopen("input.txt", "r");
    if (file)
    {
        struct list *numList = new_empty_list();
        while ((itemRead = getc(file)) != EOF)
        {
            putchar(itemRead);
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
                div(numList);
            }
            else if (itemRead == '^')
            {
                pow(numList);
            }
            else
            {
                int num = itemRead;
                add_to_front_of_list(numList, num);
            }
        }
        fclose(file);
        printf("%f \n", numList->head->data);
    }
    return 0;
}