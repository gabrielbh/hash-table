#include "GenericHashTable.h"
#include "TableErrorHandle.h"
#include <stdio.h>
#include <stdlib.h>

#define NO_FACTOR 1;
#define KEY_NOT_FOUND -1;
const int ENLARGE_FACTOR = 2;
typedef void *DataP;
typedef const void *ConstKeyP;


/**
 * A struct that represent an object.
 */
typedef struct Object
{
    void *key;
    DataP dataP;
} Object;

typedef struct Object *ObjectP;


/**
 * A struct that represent a hash table.
 */
typedef struct Table
{
    size_t tableSize;
    size_t beginningSize;
    unsigned int sizeFactor;
    CloneKeyFcn cloneKey;
    FreeKeyFcn freeKey;
    HashFcn hfun;
    PrintKeyFcn printKeyFun;
    PrintDataFcn printDataFun;
    ComparisonFcn fcomp;
    ObjectP *hashList;
} Table;


/**
 * @brief print function
 *
 */
typedef void(*PrintDataFcn)(const void *data);


/**
 * @brief Allocate memory for a hash table with which uses the given functions.
 * tableSize is the number of cells in the hash table.
 * If run out of memory, free all the memory that was already allocated by the function,
 * report error MEM_OUT to the standard error and return NULL.
 */

TableP createTable(size_t tableSize, CloneKeyFcn cloneKey, FreeKeyFcn freeKey, HashFcn hfun, PrintKeyFcn printKeyFun,
                   PrintDataFcn printDataFun, ComparisonFcn fcomp)
{
    TableP tablePointer = (TableP) malloc(sizeof(Table));
    if (!tablePointer)
    {
        reportError(MEM_OUT);
        return NULL;
    }
    tablePointer->tableSize = tableSize;
    tablePointer->beginningSize = tableSize;
    tablePointer->sizeFactor = NO_FACTOR;
    tablePointer->cloneKey = cloneKey;
    tablePointer->freeKey = freeKey;
    tablePointer->hfun = hfun;
    tablePointer->printKeyFun = printKeyFun;
    tablePointer->printDataFun = printDataFun;
    tablePointer->fcomp = fcomp;
    tablePointer->hashList = (ObjectP *) malloc(tableSize * sizeof(ObjectP));
    if (!tablePointer->hashList)
    {
        reportError(MEM_OUT);
        return NULL;
    }
    for (unsigned int i = 0; i < tableSize; ++i)
    {
        (tablePointer->hashList)[i] = (ObjectP) calloc(tableSize, sizeof(Object));
        if (!tablePointer->hashList[i])
        {
            reportError(MEM_OUT);
            return NULL;
        }
    }
    return tablePointer;
}

/**
 * Returns the minimum between two parameters of type size_t.
 */
size_t minimum(size_t a, size_t b)
{
    if (a <= b)
    {
        return a;
    }
    return b;
}

/**
 * @brief Insert an object to the table with key.
 * If all the cells appropriate for this object are full, duplicate the table.
 * If run out of memory, report
 * MEM_OUT and do nothing (the table should stay at the same situation
 * as it was before the duplication).
 * If everything is OK, return true. Otherwise (an error occured) return false;
 */
int insert(const TableP table, const void *key, DataP object)
{
    if (!table)
    {
        reportError(GENERAL_ERROR);
        return false;
    }
    int index = table->hfun(key, table->beginningSize) * table->sizeFactor;
    size_t smaller = minimum(index + table->sizeFactor, table->tableSize);
    for (unsigned int j = index; j < smaller; ++j)
    {
        if (!table->hashList[j]->dataP)
        {
            table->hashList[j]->dataP = object;
            table->hashList[j]->key = (*(table->cloneKey))(key);
            return true;
        }
    }

    table->tableSize *= ENLARGE_FACTOR;
    table->sizeFactor *= ENLARGE_FACTOR;
    ObjectP* newHashList = (ObjectP*) malloc(table->tableSize * sizeof(ObjectP));
    if (!newHashList)
    {
        reportError(MEM_OUT);
        return false;
    }

    for (unsigned int i = 0; i < table->tableSize; ++i)
    {
        newHashList[i] = (ObjectP) calloc(table->tableSize, sizeof(Object));
        if (!newHashList[i])
        {
            reportError(MEM_OUT);
            return false;
        }
    }
    // copy to 2 * i in newHashList.
    for (unsigned int i = 0; i < (table->tableSize / 2); ++i)
    {
        if (table->hashList[i]->dataP)
        {
            newHashList[i * ENLARGE_FACTOR]->dataP = table->hashList[i]->dataP;
            newHashList[i * ENLARGE_FACTOR]->key = table->cloneKey(table->hashList[i]->key);
        }
        table->freeKey(table->hashList[i]->key);
        table->freeKey(table->hashList[i]);
    }
    free(table->hashList);
    table->hashList = newHashList;
    return insert(table, key, object);
}


/**
 * @brief Search the table and look for an object with the given key.
 * If such object is found fill its cell number into arrCell (where 0 is the
 * first cell).
 * If the key was not found, fill both pointers with value of -1.
 * return pointer to the data or null
 */
DataP findData(const TableP table, const void *key, int *arrCell)
{
    if (!table)
    {
        reportError(GENERAL_ERROR);
        return NULL;
    }
    int index = table->hfun(key, table->beginningSize) * table->sizeFactor;
    size_t smaller = minimum(index + table->sizeFactor, table->tableSize);
    for (unsigned int j = index; j < smaller; ++j)
    {
        if (!table->fcomp(key, table->hashList[j]->key))
        {

            *arrCell = j;
            return table->hashList[j]->dataP;
        }
    }
    *arrCell = KEY_NOT_FOUND;
    return NULL;

}


/**
 * @brief remove an data from the table.
 * If everything is OK, return the pointer to the ejected data. Otherwise return NULL;
 */
DataP removeData(TableP table, const void *key)
{
    if (!table)
    {
        reportError(GENERAL_ERROR);
        return NULL;
    }
    int arrCell;
    DataP pointer = findData(table, key, &arrCell);
    if (pointer)
    {
        table->freeKey(pointer);
        return pointer;
    }
    return NULL;
}


/**
 * @brief return a pointer to the data that exist in the table in cell number arrCell (where 0 is the
 * first cell).
 * If such data not exist return NULL
 */
DataP getDataAt(const TableP table, int arrCell)
{
    if ((!table) || (arrCell < 0))
    {
        reportError(GENERAL_ERROR);
        return NULL;
    }
    if (table->hashList[arrCell]->dataP)
    {
        return table->hashList[arrCell]->dataP;
    }
    return NULL;
}


/**
 * @brief return the pointer to the key that exist in the table in cell number arrCell (where 0 is the
 * first cell).
 * If such key not exist return NULL
 */
ConstKeyP getKeyAt(const TableP table, int arrCell)
{
    if ((!table) || (arrCell < 0))
    {
        reportError(GENERAL_ERROR);
        return NULL;
    }
    if (table->hashList[arrCell]->key)
    {
        return table->hashList[arrCell]->key;
    }
    return NULL;
}


/**
 * @brief Print the table (use the format presented in PrintTableExample).
 */
void printTable(const TableP table)
{
    if (!table)
    {
        reportError(GENERAL_ERROR);
        return;
    }

    for (unsigned int i = 0; i < table->tableSize; ++i)
    {
        printf("[%d]\t", i);
        if (table->hashList[i]->dataP)
        {
            table->printKeyFun(table->hashList[i]->key);
            printf(",");
            table->printDataFun(table->hashList[i]->dataP);
        }
        printf("\t\n");
    }
}


/**
 * @brief Free all the memory allocated for the table.
 * It's the user responsibility to call this function before exiting the program.
 */
void freeTable(TableP table)
{
    if (!table)
    {
        reportError(GENERAL_ERROR);
        return;
    }

    for (unsigned int i = 0; i < table->tableSize; ++i)
    {
        if (table->hashList[i]->key)
        {
            table->freeKey(table->hashList[i]->key);
        }
        table->freeKey(table->hashList[i]);
    }
    free(table->hashList);
    free(table);
}
