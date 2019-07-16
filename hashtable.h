//basic hashtable 

#ifndef __hashset__
#define __hashset__
#include "vector.h"

// hashset based on vector 

typedef int (*HashSetHashFunction)(const void* element, int numBuckets); // hash function returns position in a vector
typedef int (*HashSetCompareFunction)(const void* element1, const void* element2);
typedef void (*HashSetMapFunction)(void* element, void* auxData); // vector of collidinig elements passed as element with non-zero magnitude
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
void* HashSetLookup(const hashset* h, const void* element); // returns vector of colliding elements or 0 if no such element in hashset
void HashSetMap(hashset* h, HashSetMapFunction mapfn, void* auxData);


#endif