/* Popa Marius - 313CD */
#include <stdio.h>
#include <stdlib.h>

#include "page.h"
#include "tab.h"

/**
 * This function initializes a stack.
 */
Stack* initStack() {
    Stack* stack = malloc(sizeof(*stack));

    stack->head = NULL;
    stack->size = 0;
    
    return stack;
}

int isStackEmpty(Stack* stack) {
    if (stack->head == NULL) {
        return 1;
    }
    return 0;
}

/**
 * This function adds a node to stack.
 */
void addToStack(Stack* stack, Page* page) {
    StackNode* node = malloc(sizeof(*node));

    node->page = page;
    node->next = stack->head;
    stack->head = node;
    stack->size++;
}

/**
 * This function remove a node from stack.
 */
void removeFromStack(Stack* stack) {
    if (isStackEmpty(stack)) {
        return;
    }

    StackNode* tmp = stack->head;
    stack->head = stack->head->next;
    free(tmp);
    stack->size--;
}

/**
 * This function deletes all nodes from stack,
 * but doesn't destroy the stack.
 */
void emptyStack(Stack* stack) {
    while (!isStackEmpty(stack)) {
        removeFromStack(stack);
    }
}

void destroyStack(Stack* stack) {
    emptyStack(stack);
    free(stack);
}

/**
 * This function initializes a tab-like structure.
 */
Tab* initTab(Page* pagesArray, int *last_tab_id) {
    Tab* tab = malloc(sizeof(*tab));

    tab->id = ++(*last_tab_id);
    tab->currentPage = &pagesArray[0];
    tab->backwardStack = initStack();
    tab->forwardStack = initStack();

    return tab;
}

void destroyTab(Tab* tab) {
    destroyStack(tab->backwardStack);
    destroyStack(tab->forwardStack);
    free(tab);
}
