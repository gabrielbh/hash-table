#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "MyIntFunctions.h"
#include "TableErrorHandle.h"


/**
 * @brief clone an int
 */
void* cloneInt(const void* i)
{
    assert(i != NULL);
    int* clonedInt = (int*) malloc(sizeof(int));
    if (! clonedInt)
    {
        reportError(MEM_OUT);
        return NULL;
    }
    *clonedInt = *(int*) i;
    return clonedInt;
}


/**
 * @brief free an int
 */
void freeInt( void* i)
{
    assert(i != NULL);
    if (i)
    {
        free(i);
    }
}


/**
 * @brief hash value of key for HashTable with size tableSize
 *  assuming key pointer to an int
 * @return number between 0-(tableSize-1)

 */
int intFcn (const void* key, size_t tableSize)
{
    assert(key != NULL);
    return ((*(int*) key % (int) tableSize) + (int) tableSize) % (int) tableSize;
}



/**
 * @brief print a string
 *  assuming key pointer to an int
 */
void intPrint (const void* key)
{
    assert(key != NULL);
    printf("%d", *(int*) key);
}



/**
 *  intCompare - pointer to int comparison function:
 * @brief compare 2 ints
 *   returns zero int if both ints are equal, otherwise
 *   returns non-zero.
 *
 */
int intCompare (const void* firstInt, const void* secondInt)
{
    if (firstInt == NULL || secondInt == NULL)
    {
        return 1;
    }
    if (*(int*) firstInt == *(int*) secondInt)
    {
        return 0;
    }
    return 1;
}
