/* Popa Marius - 313CD */
#ifndef BROWSER_H
#define BROWSER_H

#include "tab.h"

struct tabNode;

typedef struct tabNode{
    Tab* tab;
    struct tabNode* next;
    struct tabNode* prev;
}tabNode, *tabList;

typedef struct Browser {
    struct Tab* current;
    tabList list;
}Browser;

Browser* createBrowser(Page* array, int* last_tab_id);
tabNode* findTab(Browser* b, int id);
void addToFinaltabList(Browser* b, Tab* tab);
tabNode* createtabNode(Tab* tab);
int rmvCrttabNode(Browser* b);
void destroyBrowser(Browser* b);
tabNode* findtabNode(Browser *b, int tabId);

#endif