/* Popa Marius - 313CD */
#ifndef PAGE_H
#define PAGE_H

#define maxPages 50

typedef struct Page{
    int id;
    char url[50];
    char *description;
}Page;

void initArray(Page* array, int* size);
void addPageToArray(Page* array, int* size, int id, char* url, char* description);
Page* findPage(Page* array, int size , int id);
void destroyPagesArray(Page* array, int size);

#endif