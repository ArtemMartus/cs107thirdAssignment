#include "hashtable.h"
#include <assert.h> //assert
#include <stdlib.h> // malloc free 
#include <stdio.h> // printf

void vectorFree(void* element){
	assert(element != 0);
	vector* v = (vector*) element;
	VectorDispose(v);
	free(v);
}

void HashSetNew(hashset* h, int elementSize, int numBuckets, HashSetHashFunction hashfn, 
	HashSetCompareFunction comparefn, HashSetFreeFunction freefn){
	assert(h != 0);
	assert(elementSize > 0);
	assert(numBuckets > 0);
	assert(hashfn != 0);
	assert(comparefn != 0);

	h->elementSize = elementSize;
	h->numBuckets = numBuckets;
	h->hashfn = hashfn;
	h->comparefn = comparefn;

	#ifndef DNDEBUG
	int memoryAllocated = sizeof(vector) + sizeof(vector) * numBuckets;
	#endif
	h->base = malloc(sizeof(vector));
	assert(h->base != 0);

	VectorNew(h->base, sizeof(vector), 0, numBuckets); //vector of vectors

	for (int i = 0; i < numBuckets; ++i){
		#ifndef DNDEBUG
		memoryAllocated += sizeof(vector);
		#endif
		vector* v = malloc(sizeof(vector));
		assert(v!=0);
		VectorNew(v, elementSize, freefn, 0);
		VectorAppend(h->base, (void*)v);
	}

	#ifndef DNDEBUG
	printf("Hashset initialized with %d bytes\n", memoryAllocated);
	#endif
}

void HashSetDispose(hashset* h){
	assert(h != 0);
	assert(h->base != 0);

	VectorDispose(h->base);
	free(h->base);

	#ifndef DNDEBUG
	printf("Hashset disposed\n");
	#endif
}

int HashSetCount(const hashset* h){
	assert(h != 0);
	assert(h->base != 0);
	int count = 0;
	for (int i = 0; i < h->numBuckets; ++i)
	{
		vector* v = VectorNth(h->base, i);
		count += VectorLength(v);
	}
	return count;
}

void HashSetEnter(hashset* h, const void* element){
	assert(h != 0);
	assert(h->base != 0);
	assert(element != 0);
	assert(h->comparefn != 0);
	assert(h->hashfn != 0);
	vector* bucket = VectorNth(h->base,h->hashfn(element,h->numBuckets));
	VectorAppend(bucket,element);
	if(VectorLength(bucket) > 1)
		VectorSort(bucket,h->comparefn);
}

void* HashSetLookup(const hashset* h, const void* element){
	assert(h != 0);
	assert(h->base != 0);
	assert(element != 0);
	assert(h->hashfn != 0);
	vector* bucket = VectorNth(h->base,h->hashfn(element,h->numBuckets));
	if(VectorLength(bucket)>0)
		return bucket;
	return 0;
}

void HashSetMap(hashset* h, HashSetMapFunction mapfn, void* auxData){
	assert(h != 0);
	assert(h->base != 0);
	assert(mapfn != 0);
	for (int i = 0; i < h->numBuckets; ++i)
	{
		vector* v = VectorNth(h->base, i);
		if(VectorLength(v) > 0)
			mapfn(v, auxData);
	}
	#ifndef DNDEBUG
	printf("Applied some function to hashset\n");
	#endif
}