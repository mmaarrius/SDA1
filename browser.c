/* Popa Marius - 313CD */
#include <stdio.h>
#include <stdlib.h>
#include "browser.h"
#include "tab.h"
#include "page.h"

/**
 * This function creates a new browser-like structure.
 * (doubly list with sentinel)
 */
Browser* createBrowser(Page* array, int* last_tab_id) {
    Browser* b = malloc(sizeof(*b));

    Tab* tab = initTab(&array[0], last_tab_id);
    tabNode* node = createtabNode(tab);
    tabNode* sentinel = createtabNode(NULL);

    b->list = sentinel;
    b->current = node->tab;
    sentinel->next = node;
    sentinel->prev = node;
    node->prev = sentinel;
    node->next = sentinel;

    return b;
}

/**
 * This function adds a new node to final
 * of browser list.
 */
void addToFinaltabList(Browser* b, Tab* tab) {
    tabNode *node = createtabNode(tab);

    node->next = b->list;
    node->prev = b->list->prev;
    node->prev->next = node;
    b->list->prev = node;
}

/**
 * This function finds a node in list.
 */
tabNode* findtabNode(Browser *b, int tabId) {
    tabNode* tmp = b->list->next;
    while(tmp != b->list) {
        if(tmp->tab->id == tabId) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

/**
 * This function creates a new node-like 
 * structure for browser list. 
 */
tabNode* createtabNode(Tab* tab) {
    tabNode* node = malloc(sizeof(*node));

    node->tab = tab;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

/**
 * This function deletes current tab
 * if it's not sentinel or id=0.
 */
int rmvCrttabNode(Browser* b) {
    if (b->current == b->list->tab || b->current->id == 0) {
        return 0;
    }

    tabNode* tmp = findtabNode(b, b->current->id);
    b->current = tmp->prev->tab;
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;

    destroyTab(tmp->tab);
    free(tmp);
    return 1;
}

void destroyBrowser(Browser* b) {
    tabNode* tmp = b->list;
    // pt conditia de la while
    b->list->prev->next = NULL;
    b->list = b->list->next;
    free(tmp);


    while (b->list != NULL) {
        tmp = b->list;
        b->list = b->list->next;
        destroyTab(tmp->tab);
        free(tmp);
    }
    free(b);
}