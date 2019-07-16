//basic hashtable 

#ifndef __hashset__
#define __hashset__
#include "vector.h"

// hashset based on vector 

typedef int (*HashSetHashFunction)(const void* element, int numBuckets); // hash function returns position in a vector
typedef int (*HashSetCompareFunction)(const void* element1, const void* element2);
typedef void (*HashSetMapFunction)(void* element, void* auxData);
typedef void (*HashSetFreeFunction)(void* element);

typedef struct{
	int elementSize;
	int numBuckets;
	HashSetHashFunction hashfn;
	HashSetCompareFunction comparefn;
	vector* base;
} hashset;

void HashSetNew(hashset* h, int elementSize, int numBuckets, HashSetHashFunction hashfn, 
	HashSetCompareFunction comparefn, HashSetFreeFunction freefn);
void HashSetDispose(hashset* h);
int HashSetCount(const hashset* h);
void HashSetEnter(hashset* h, const void* element);
void* HashSetLookup(const hashset* h, const void* element);
void HashSetMap(hashset* h, HashSetMapFunction mapfn, void* auxData);


#endif