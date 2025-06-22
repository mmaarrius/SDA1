/* Popa Marius - 313CD */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "tab.h"
#include "browser.h"

#define BUFFER_SIZE 100

void processCommand(char* command, Browser *b, Page* arr, int size, int *last_tab_id);
void newTabCmd(Browser *b, Page* array, int *last_tab_id);
void printCmd(Browser *b);
void closeCmd(Browser* b);
void openTabCmd(Browser* b, char* line);
void nextTabCmd(Browser* b);
void prevTabCmd(Browser* b);
void pageCmd(Browser* b, char* cmd, Page* array, int size);
void forwardHistory(StackNode* SNode, FILE* f_out);
void backwardHistory(StackNode* SNode, FILE* f_out);
void historyCmd(Browser* b, char* cmd);
void backwardCmd(Browser* b);
void forwardCmd(Browser* b);

int main() {
    // initialize the array with web pages
    Page* PagesArray = calloc(BUFFER_SIZE, sizeof(Page));
    int arrSize = 0;
    initArray(PagesArray, &arrSize);

    // will be increased to 0 in the function
    int last_tab_id = -1;

    // initialize the browser
    Browser* b = createBrowser(PagesArray, &last_tab_id);

    int commands = 0;
    char buffer[BUFFER_SIZE];

    FILE *f = fopen("tema1.in", "r");
    if (!f) {
        perror("Error opening file.\n");
        destroyBrowser(b);
        destroyPagesArray(PagesArray, arrSize);
        return 1;
    }

    // read number of total web pages
    fgets(buffer, BUFFER_SIZE, f);
    int readPages = atoi(buffer);

    // read pages
    for (int i = 0; i < readPages; i++) {
        fgets(buffer, BUFFER_SIZE, f);
        int id = atoi(buffer);

        fgets(buffer, BUFFER_SIZE, f);
        char url[50];
        strcpy(url, buffer);

        fgets(buffer, BUFFER_SIZE, f);
        char description[BUFFER_SIZE];
        strcpy(description, buffer);

        addPageToArray(PagesArray, &arrSize, id, url, description);
    }

    // read number of commands
    fgets(buffer, BUFFER_SIZE, f);
    commands = atoi(buffer);

    // just empty the file
    FILE *f_out = fopen("tema1.out", "w");
    if (!f_out) {
        perror("Error opening file.\n");
        destroyBrowser(b);
        destroyPagesArray(PagesArray, arrSize);
        fclose(f);
        return 1;
    }
    fclose(f_out);

    // read all commands
    for (int i = 0; i < commands; i++) {
        fgets(buffer, BUFFER_SIZE, f);

        processCommand(buffer, b, PagesArray, arrSize, &last_tab_id);
    }


    destroyBrowser(b);
    destroyPagesArray(PagesArray, arrSize);
    fclose(f);
    return 0;
}

/**
 * 
 */
void processCommand(char* command, Browser *b, Page* arr, int arrSize, int *last_tab_id) {
    if (strstr(command, "NEW_TAB")) {
        newTabCmd(b, arr, last_tab_id);
    } else if (strstr(command, "CLOSE")) {
        closeCmd(b);
    } else if (strstr(command, "OPEN")) {
        openTabCmd(b, command);
    } else if (strstr(command, "NEXT")) {
        nextTabCmd(b);
    } else if (strstr(command, "PREV")) {
        prevTabCmd(b);
    } else if (strstr(command, "PAGE")) {
        pageCmd (b, command, arr, arrSize);
    } else if (strstr(command, "PRINT_HISTORY")) {
        historyCmd(b, command);
    } else if (strstr(command, "PRINT")) {
        printCmd(b);
    } else if (strstr(command, "BACKWARD")) {
        backwardCmd(b);
    } else if (strstr(command, "FORWARD")) {
        forwardCmd(b);
    }
}

/**
 * This function makes a new tab in browser.
 */
void newTabCmd (Browser *b, Page* arr, int *last_tab_id) {
    Tab* tab = initTab(&arr[0], last_tab_id);
    addToFinaltabList(b, tab);
    b->current = b->list->prev->tab;
}

/**
 * Print all tabs id.
 */
void printCmd(Browser *b) {
    FILE *f_out = fopen("tema1.out", "a");
    if (!f_out) {
        printf("Error opening file\n");
        return;
    } 
    tabNode *tmp = findtabNode(b, b->current->id);
    do
    {
        fprintf(f_out, "%d ", tmp->tab->id);
        tmp = tmp->next;
        if (tmp == b->list) {
            tmp = tmp->next;
        }
    }while(tmp->tab->id != b->current->id);
    fprintf(f_out, "\n");
    fprintf(f_out, "%s\n", tmp->tab->currentPage->description);
    fclose(f_out);
}

/**
 * Delete current tab from browser.
 */
void closeCmd(Browser* b) {
    FILE *f_out = fopen("tema1.out", "a");
    if (!f_out) {
        printf("Error opening file\n");
        return;
    } 
    if (rmvCrttabNode(b) == 0) {
        fprintf(f_out, "403 Forbidden\n");
    }
    fclose(f_out);
}

/**
 * This function selects a certain tab in browser.
 */
void openTabCmd(Browser* b, char* command) {
    char id_str[3];
    command[strlen(command)] = '\0';
    sscanf(command, "OPEN %s", id_str);
    int id = atoi(id_str);

    tabNode* tmp = findtabNode(b, id);
    if (tmp == NULL) {
        FILE *f_out = fopen("tema1.out", "a");
        if (!f_out) {
            printf("Error opening file\n");
            return;
        }
        fprintf(f_out, "403 Forbidden\n");
        fclose(f_out);
    } else {
        b->current = tmp->tab;
    }
}

/**
 * Goes to next tab.
 */
void nextTabCmd(Browser* b) {
    tabNode* currNode = findtabNode(b, b->current->id);
    b->current = currNode->next->tab;
    if (b->current == b->list->tab) {
        b->current = currNode->next->next->tab;
    }
}

/**
 * Goes to previous tab.
 */
void prevTabCmd(Browser* b) {
    tabNode* currNode = findtabNode(b, b->current->id);
    b->current = currNode->prev->tab;
    if (b->current == b->list->tab) {
        b->current = currNode->prev->prev->tab;
    }
}

/**
 * Add a new page in the tab.
 */
void pageCmd(Browser* b, char* cmd, Page* array, int size) {
    char id_str[4];
    cmd[strlen(cmd)] = '\0';
    sscanf(cmd, "PAGE %s", id_str);
    int id = atoi(id_str);

    Page* page = findPage(array, size, id);


    if (page == NULL) {
        FILE *f_out = fopen("tema1.out", "a");
        if (!f_out) {
            printf("Error opening file\n");
            return;
        }
        fprintf(f_out, "403 Forbidden\n");
        fclose(f_out);
    } else {
        Tab* currTab = b->current;
        addToStack(currTab->backwardStack, currTab->currentPage);
        currTab->currentPage = page;
        emptyStack(currTab->forwardStack);
    }
}

/**
 * Shows all pages after current page.
 */
void forwardHistory(StackNode* SNode, FILE* f_out) {
    if (SNode == NULL) {
        return;
    }
    forwardHistory(SNode->next, f_out);
    fprintf(f_out, "%s\n", SNode->page->url);
}

/**
 * Shows all pages before current page.
 */
void backwardHistory(StackNode* SNode, FILE* f_out) {
    if (SNode == NULL) {
        return;
    }
    fprintf(f_out, "%s\n", SNode->page->url);
    backwardHistory(SNode->next, f_out);
}

/**
 * Shows all pages from tab history.
 */
void historyCmd(Browser* b, char* cmd) {
    char id_str[4];
    sscanf(cmd, "PRINT_HISTORY %s", id_str);
    int id = atoi(id_str);

    FILE *f_out = fopen("tema1.out", "a");
    if (!f_out) {
        perror("Error opening file\n");
        return;
    }

    tabNode* node = findtabNode(b, id);
    if (node == NULL) {
        fprintf(f_out, "403 Forbidden\n");
        fclose(f_out);
        return;
    }

    forwardHistory(node->tab->forwardStack->head, f_out);
    fprintf(f_out, "%s\n", node->tab->currentPage->url);
    backwardHistory(node->tab->backwardStack->head, f_out);
    fclose(f_out);
}

/**
 * This function lets us to go back to
 * previous page.
 */
void backwardCmd(Browser* b) {
    Tab* tab = b->current;
    if (tab->backwardStack->head == NULL) {
        FILE *f_out = fopen("tema1.out", "a");
        if (!f_out) {
            printf("Error opening file\n");
            return;
        }
        fprintf(f_out, "403 Forbidden\n");
        fclose(f_out);
        return;
    }
    addToStack(tab->forwardStack, tab->currentPage);
    tab->currentPage = tab->backwardStack->head->page;
    removeFromStack(tab->backwardStack);
}

/**
 * This function lets us going to next page.
 */
void forwardCmd(Browser* b) {
    Tab* tab = b->current;
    if (tab->forwardStack->head == NULL) {
        FILE *f_out = fopen("tema1.out", "a");
        if (!f_out) {
            printf("Error opening file\n");
            return;
        }
        fprintf(f_out, "403 Forbidden\n");
        fclose(f_out);
        return;
    }
    addToStack(tab->backwardStack, tab->currentPage);
    tab->currentPage = tab->forwardStack->head->page;
    removeFromStack(tab->forwardStack);
}