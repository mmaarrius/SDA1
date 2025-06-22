/* Popa Marius - 313CD */
#ifndef TAB_H
#define TAB_H

#include "page.h"

typedef struct StackNode {
    Page* page;
    struct StackNode* next;
}StackNode;

typedef struct Stack {
    StackNode* head;
    int size;
}Stack;

typedef struct Tab {
    int id;
    Page* currentPage;
    Stack* backwardStack;
    Stack* forwardStack;
}Tab;

Stack* initStack();
int isStackEmpty(Stack* stack);
void addToStack(Stack* stack, Page* page);
void removeFromStack(Stack* stack);
void destroyStack(Stack* stack);
Tab* initTab(Page* pagesArray, int *last_tab_id);
void emptyStack(Stack* stack);
void destroyTab(Tab* tab);

#endif