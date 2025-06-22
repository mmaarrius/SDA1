/* Popa Marius - 313CD */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"page.h"

/**
 * This function adds default page to array.
 */
void initArray(Page* array, int *size) {
    array[0].id = 0;
    strcpy(array[0].url, "https://acs.pub.ro/");
    array[0].description = calloc(strlen("Computer Science") + 1, 1);
    strcpy(array[0].description, "Computer Science");
    (*size)++;
}

/**
 * This function adds a new page to array of pages.
 */
void addPageToArray(Page* array, int *size, int id, char *url, char *description) {
    if (*size == maxPages) {
        printf("403 Forbidden\n");
        return;
    }

    array[*size].id = id;
    url[strlen(url)] = '\0';
    strcpy(array[*size].url, url);
    
    description[strlen(description)] = '\0';
    array[*size].description = calloc(strlen(description) + 1, 1);  // +1 for '\0'
    strcpy(array[*size].description, description);

    (*size)++;
}

/**
 * This function finds properly page.
 */
Page* findPage(Page* array, int size , int id) {
    for (int i = 0; i < size; i++) {
        if (array[i].id == id) {
            return &array[i];
        }
    }
    return NULL;
}

/**
 * This function free dinamic memory of all the pages from array.
 */
void destroyPagesArray(Page* array, int size) {
    for(int i = 0; i < size; i++) {
        free(array[i].description);
    }
    free(array);
} 
