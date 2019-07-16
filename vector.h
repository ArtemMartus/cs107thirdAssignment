//basic vector container

#ifndef __vector__
#define __vector__

#include <stdlib.h>
#include <stdio.h>

typedef char bool;

typedef int (*VectorCompareFunction)(const void* element1, const void* element2);
typedef void (*VectorMapFunction)(void* element, void* auxData);
typedef void (*VectorFreeFunction)(void* element);

typedef struct {
	void* base;
	int size; // last element
	int capacity; // bytes allocated
	int elementSize;
	VectorFreeFunction freefn;
} vector;

void VectorNew(vector* v, int elementSize, VectorFreeFunction freefn, 
	int initialAllocation); // initialAllocation means initial element count
void VectorDispose(vector* v);
int VectorLength(const vector* v);
void* VectorNth(const vector* v, int position);
void VectorInsert(vector* v, const void* element, int position);
void VectorAppend(vector* v, const void* element);
void VectorReplace(vector* v, const void* element, int position);
void VectorDelete(vector* v, int position);
int VectorSearch(const vector* v, const void* key, VectorCompareFunction searchfn, 
	int startIndex, bool isSorted);
void VectorSort(vector* v, VectorCompareFunction comparefn);
// VectorMap applies mapfn to each vector element with auxData if needed
void VectorMap(vector* v, VectorMapFunction mapfn, void* auxData); 

#endif