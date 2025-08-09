#ifndef E_LIST_H
#define E_LIST_H
/*
USE: define E_LIST_IMPLEMENTATION the first time elist is included.
elist contains all the functionality needed to use dynamic arrays in c.

Keep in mind that since templates are not in C, you will need to dereference any output from a elist_get approprately
this means converting the void* to the type*, then dereferencing that type IF NESSESARY
(you will not need to dereference again when getting a char* in example)

e.g:
(int)
*(int*)elist_get(&elist, index);
*/
#include "string.h"
#include "stdlib.h"
#include "econstants.h"

// IMPORTANT, DO NOT CREATE A POINTER TO AN UNINITIALIZED LIST, causes seg-faults
typedef struct{
    size_t size;
    size_t maxSize;
    size_t objSize;
    // DO NOT MANUALLY EDIT THE "mem" ATTRIBUTE, UNLESS YOU REALLY KNOW WHAT YOU ARE DOING!!!
    void** mem;
} eList;

// Initializes a new list. Should only be used once
int elist_create(eList * list, size_t objSize);
// Deletes the list, and the pointers inside the list
void elist_delete(eList * list);
// Adds a new item to the end of the list
int elist_add(eList * list, const void * object);
// Returns a pointer to the item in the list
void* elist_get(eList * list, size_t index); 
// Expands the array by a number of objects (NOT BYTES)
int elist_expand(eList * list, size_t numOfObjects); 
// Inserts an elements at a specific point of the USED list
int elist_replace(eList * list, const void * object, size_t index); 
// Removes an item from the list
int elist_remove(eList * list, size_t index);
// Removes items from a list from start to stop (exclusive)
int elist_remove_range(eList * list, size_t start, size_t stop);
// Clears items from a list. This function does not free the list
void elist_clear(eList * list);

#ifdef E_LIST_IMPLEMENTATION

int elist_create(eList * list, size_t objSize){
    list->mem = (void**)malloc(sizeof(void*)*ESTD_DEFAULT_SIZE);
    if (list->mem == NULL) return ESTD_FAILURE;

    for (size_t i = 0; i < ESTD_DEFAULT_SIZE; ++i){
        list->mem[i] = malloc(objSize);
        if (list->mem[i] == NULL){
            elist_delete(list);
            return ESTD_FAILURE;
        }
        ++list->size; // Only increased so in case of failure, the list can be properly deallocated
    }

    list->size = 0;
    list->maxSize = ESTD_DEFAULT_SIZE;
    list->objSize = objSize;
    return ESTD_SUCCESS;
}

void elist_delete(eList * list){
    for (size_t i = 0; i < list->size; ++i){
        free(list->mem[i]);
    }
    free(list->mem);

    list->size = 0;
    list->maxSize = 0;
    list->objSize = 0;
    list->mem = NULL; // Deal with dangling pointer or something
}

int elist_add(eList * list, const void * object){
    if (list->size >= list->maxSize){
        if (elist_expand(list, list->maxSize) == ESTD_FAILURE){ // Expands the list by the current max size
            return ESTD_FAILURE;
        }
    }
    // Adding the object to the end of the list
    memcpy(list->mem[list->size], object, list->objSize);
    ++list->size;
    return ESTD_SUCCESS;
}

void* elist_get(eList * list, size_t index){
    if (index >= list->size){
        return NULL;
    }
    else{
        return list->mem[index];
    }
}

int elist_expand(eList * list, size_t numOfObjects){
    // Creating the new memory block
    void ** newMemBlock = (void**)malloc(sizeof(void*)*(list->maxSize+numOfObjects)); // Doubles size of array
    if (newMemBlock == NULL) return ESTD_FAILURE;
    for (size_t i = list->size; i < list->maxSize+numOfObjects; ++i){
        newMemBlock[i] = malloc(list->objSize);
        if (newMemBlock[i] == NULL){
            free(newMemBlock);
            return ESTD_FAILURE;
        }
    }

    // Copying values from old block to new block, then deallocating the pointers in the old memory block
    memcpy(newMemBlock, list->mem, sizeof(void*)*list->size); // Faster then using memcpy, then freeing the old memory etc...
    free(list->mem);
    list->mem = newMemBlock;
    list->maxSize = list->maxSize+numOfObjects; // Changing the new maximum size
    newMemBlock = NULL; // Handle the dangling pointer
    return ESTD_SUCCESS;
}

int elist_replace(eList * list, const void * object, size_t index){
    if (index < list->size){
        memcpy(list->mem[index], object, list->objSize);
        return ESTD_SUCCESS;
    }
    return ESTD_FAILURE;
}

int elist_remove(eList * list, size_t index){
    if (index < list->size){
        for (size_t i = index; i < list->size-1; ++i){
            memcpy(list->mem[i], list->mem[i+1], list->objSize);
        }

        memset(list->mem[list->size-1], 0, list->objSize);
        --list->size;
        return ESTD_SUCCESS;
    }
    return ESTD_FAILURE;
}

int elist_remove_range(eList * list, size_t start, size_t stop){
    if (start < list->size && stop <= list->size && start < stop){
        for(size_t i = start; i < stop; ++i){
            memset(list->mem[i], 0, list->objSize);
        }

        size_t range = (stop-start);
        for (size_t i = start; i+range < list->size; ++i){
            memcpy(list->mem[i], list->mem[i+range], list->objSize);
        }

        list->size -= range;
        return ESTD_SUCCESS;
    }
    return ESTD_FAILURE;
}

void elist_clear(eList * list){
    elist_remove_range(list, 0, list->size);
}

#endif
#endif