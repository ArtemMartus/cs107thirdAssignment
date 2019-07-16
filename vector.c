#include "vector.h"
#include <assert.h> // assert
#include <stdlib.h> // malloc free realloc
#include <string.h> //memcpy
#include <stdio.h> //printf

void VectorNew(vector* v, int elementSize, VectorFreeFunction freefn, 
	int initialAllocation){
	assert(v!=0); 
	assert(elementSize>=1); 
	assert(initialAllocation>=0);
	v->capacity = initialAllocation * elementSize;
	v->base = malloc(v->capacity);
	assert(v->base != 0);
	v->size = 0;
	v->elementSize = elementSize;
	v->freefn = freefn;
	#ifndef DNDEBUG
	printf("Vector initial values set, allocated %d bytes\n",v->capacity);
	#endif
}

void disposeElement(void* element, void* auxData){
	assert(element!=0); 
	assert(auxData!=0);
	VectorFreeFunction fn = (VectorFreeFunction)auxData;
	#ifndef DNDEBUG
	printf("Disposing element with provided free function\n");
	#endif
	fn(element);
}

void VectorDispose(vector* v){
	assert(v!=0);
	if(v->freefn != 0){
		VectorMap(v, disposeElement, (void*)v->freefn );
	}
	if(v->base != 0)
		free(v->base);
	#ifndef DNDEBUG
	printf("Vector base disposed\n");
	#endif
}

int VectorLength(const vector* v){
	assert(v != 0); 
	assert(v->size >= 0);
	return v->size;
}

void* VectorNth(const vector* v, int position){
	assert(v != 0); 
	assert(position >= 0); 
	assert(v->base != 0);
	if(position >= v->size) return 0; // out of bounds
	return v->base + (v->elementSize * position);
}

void VectorInsert(vector* v, const void* element, int position){
	assert(v!=0);
	assert(element!=0);
	assert(position>=0);
	assert(position<=v->size);

	int spaceFor = v->capacity / v->elementSize;
	if(spaceFor <= v->size){
		v->capacity += 3 * v->elementSize;
		v->base = realloc(v->base, v->capacity);
		assert(v->base != 0);
		#ifndef DNDEBUG
		printf("Vector base reallocated with new capacity of %d bytes\n",v->capacity);
		#endif
	}

	void* bufferOne = malloc(v->elementSize);
	void* bufferTwo = malloc(v->elementSize);
	assert(bufferOne != 0);
	assert(bufferTwo != 0);

	memcpy(bufferOne,element,v->elementSize); // write element to bufferOne
	int toShift = v->size - position;

	while(toShift >= 0){ //shift to the right
		memcpy(bufferTwo,v->base + (v->size - toShift) * v->elementSize, v->elementSize);
		memcpy(v->base + (v->size - toShift) * v->elementSize, bufferOne ,v->elementSize);
		memcpy(bufferOne, bufferTwo, v->elementSize);
		toShift--;
	}

	free(bufferOne);
	free(bufferTwo);

	memcpy(v->base + position * v->elementSize, element, v->elementSize);
	v->size++;

	#ifndef DNDEBUG
	printf("Element inserted, new vector size is %d\n",v->size);
	#endif
}

void VectorAppend(vector* v, const void* element){
	assert(v != 0); 
	assert(element!=0);

	VectorInsert(v,element,v->size);
}

void VectorReplace(vector* v, const void* element, int position){
	assert(v != 0); 
	assert(element != 0); 
	assert(position >=0); 
	assert(position < v->size);
	void* pastElement = VectorNth(v,position);
	assert(pastElement != 0);
		
	if(v->freefn != 0)
		v->freefn(pastElement);

	memcpy(v->base + position * v->elementSize, element, v->elementSize);

	#ifndef DNDEBUG
	printf("Element %d replaced\n",position);
	#endif
}

void VectorDelete(vector* v, int position){
	assert(v != 0); 
	assert(position >=0); 
	assert(position < v->size);
	void* pastElement = VectorNth(v,position);
	assert(pastElement != 0);
	if(v->freefn != 0)
		v->freefn(pastElement);
	for (int i = position+1; i < v->size; ++i){//shift right elements to the left
		memcpy(v->base + (i-1) * v->elementSize, 
			v->base + i * v->elementSize, 
			v->elementSize);
	}
	v->size--;

	#ifndef DNDEBUG
	printf("Element %d deleted, new size %d\n",position,v->size);
	#endif
}

int VectorSearch(const vector* v, const void* key, VectorCompareFunction searchfn, 
	int startIndex, bool isSorted){
	assert(v != 0);
	assert(v->base != 0);
	assert(key != 0);
	assert(searchfn != 0);
	assert(startIndex >= 0);
	assert(v->size > 0);

	if(isSorted){
		#ifndef DNDEBUG
		printf("Binary search started..");
		#endif
		int min = 0;
		int max = v->size + 1;
		int mid = (min + max) / 2;	
		void *el = VectorNth(v,mid);
		//use binary search
		while ( max - min >= 1 ){
			int searchRet = searchfn(key,el);
			if( searchRet == 0){
				#ifndef DNDEBUG
				printf("found at %d\n",mid);
				#endif
				return mid;
			} else if ( searchRet > 0 ) {
				min = mid - 1;
			} else {
				max = mid;
			}
			mid = (min + max) / 2;
			el = VectorNth(v,mid);
		}
	} else {
		#ifndef DNDEBUG
		printf("Linear search started..");
		#endif
		// use linear search
		for(int i = startIndex; i < v->size; ++i){
			if(searchfn(key,v->base + i * v->elementSize) == 0){
				#ifndef DNDEBUG
				printf("found at %d\n",i);
				#endif
				return i;
			}
		}
	}
	#ifndef DNDEBUG
	printf("not found\n");
	#endif
	return -1;
}

void VectorSort(vector* v, VectorCompareFunction comparefn){
	assert(v != 0); 
	assert(comparefn != 0);
	assert(v->base!= 0);

	int retVal = mergesort(v->base, v->size, v->elementSize, comparefn);

	#ifndef DNDEBUG
	if(retVal == 0)
	// implement quick sort
		printf("Vector sorted\n");
	else
		printf("Vector NOT sorted\n");
	#endif
}

void VectorMap(vector* v, VectorMapFunction mapfn, void* auxData){
	assert(v != 0);
	assert(mapfn != 0);
	for (int i = 0; i < v->size; ++i){
		mapfn(v->base + i * v->elementSize, auxData);
	}
	#ifndef DNDEBUG
	printf("Applied some function to vector\n");
	#endif
}