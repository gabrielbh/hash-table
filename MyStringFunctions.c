#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "MyStringFunctions.h"
#include "TableErrorHandle.h"

/**
 * @brief clone a string
 */
void* cloneStr(const void*  s)
{
    assert(s != NULL);
    char* clonedStr = (char *) malloc(sizeof(char));
    if (! clonedStr)
    {
        reportError(MEM_OUT);
        return NULL;
    }
    strcpy(clonedStr, (char*) s);
    return clonedStr;
}


/**
 * @brief free an string
 */
void freeStr(void* s)
{
    assert(s != NULL);
    free(s);
}


/**
 * @brief hash value of key for HashTable with size tableSize
 *  assuming key pointer to string
 * @return number between 0-(tableSize-1)

 */
int strFcn (const void*  s, size_t tableSize)
{
    assert(s != NULL);
    int asciiSum = 0;
    char* str = (char*) s;
    for (unsigned int i = 0; i < strlen(str); ++i)
    {
        asciiSum += str[i];
    }
    int answer = asciiSum % (int) tableSize;
    return answer;
}


/**
 * @brief print a string
 *  assuming key pointer to string
 *
 */
void strPrint (const void*  s)
{
    assert(s != NULL);
    char* str = (char*) s;
    for (unsigned int i = 0; i < strlen(str); ++i)
    {
        printf("%c", str[i]);
    }
}


/**
 *  strCompare - pointer to int comparison function:
 * @brief compare 2 strings
 *   returns zero int if both strings are equal, otherwise
 *   returns non-zero.
 */
int strCompare(const void*  s1, const void*  s2)
{
    assert(s1 != NULL && s2 != NULL);
    return strcmp((char*) s1, (char*) s2);
}
