CC= gcc
CFLAGS= -Wvla -std=c99 -Wextra -g -Wall -c -DNDEBUG
LIB= ar rcs
RM= -rm


#Run Target

run: HashIntSearch
	./HashIntSearch 1 0

#Executeabla File

HashIntSearch: HashIntSearch.o libgenericHashTable.a TableErrorHandle.o MyIntFunctions.o
	$(CC) HashIntSearch.o libgenericHashTable.a TableErrorHandle.o MyIntFunctions.o -o HashIntSearch

HashStrSearch: HashStrSearch.o libgenericHashTable.a TableErrorHandle.o MyStringFunctions.o
	$(CC) HashStrSearch.o libgenericHashTable.a TableErrorHandle.o MyStringFunctions.o -o HashStrSearch

GenericHashTable: GenericHashTable.o
	$(LIB) libgenericHashTable.a GenericHashTable.o

libgenericHashTable.a: GenericHashTable.o
	$(LIB) libgenericHashTable.a GenericHashTable.o



#Object File
HashIntSearch.o: HashIntSearch.c TableErrorHandle.h
	$(CC) $(CFLAGS) HashIntSearch.c -o HashIntSearch.o

HashStrSearch.o: HashStrSearch.c TableErrorHandle.h
	$(CC) $(CFLAGS) HashStrSearch.c -o HashStrSearch.o

GenericHashTable.o: GenericHashTable.c TableErrorHandle.h
	$(CC) $(CFLAGS) GenericHashTable.c  -o GenericHashTable.o


TableErrorHandle.o: TableErrorHandle.c
	$(CC) $(CFLAGS) TableErrorHandle.c -o TableErrorHandle.o

MyIntFunctions.o: MyIntFunctions.c TableErrorHandle.h
	$(CC) $(CFLAGS) MyIntFunctions.c -o MyIntFunctions.o

MyStringFunctions.o: MyStringFunctions.c TableErrorHandle.h
	$(CC) $(CFLAGS) MyStringFunctions.c -o MyStringFunctions.o

#Other Traget


clean:
	$(RM) -f *.o
	$(RM) HashIntSearch
	$(RM) HashStrSearch
	$(RM) libgenericHashTable.a


.PHONY: clean




