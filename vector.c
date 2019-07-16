#include "vector.h"
#include <assert.h> // assert
#include <stdlib.h> // malloc free realloc
#include <string.h> //memcpy

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
}

void disposeElement(void* element, void* auxData){
	assert(element!=0); 
	assert(auxData!=0);
	VectorFreeFunction fn = (VectorFreeFunction)auxData;
	fn(element);
}

void VectorDispose(vector* v){
	assert(v!=0);
	if(v->freefn != 0){
		VectorMap(v, disposeElement, (void*)v->freefn );
	}
	free(v);
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

}

void VectorAppend(vector* v, const void* element){
	assert(v != 0); 
	int spaceFor = v->capacity / v->elementSize;
	if(spaceFor <= v->size){
		v->capacity += 3 * v->elementSize;
		v->base = realloc(v->base, v->capacity);
		assert(v->base != 0);
	}
	memcpy(v->base + v->size * v->elementSize, element, v->elementSize);
	v->size++;
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
}

int VectorSearch(const vector* v, const void* key, VectorCompareFunction searchfn, 
	int startIndex, bool isSorted){
	assert(v != 0);
	assert(key != 0);
	assert(searchfn != 0);
	assert(startIndex >= 0);

	if(isSorted){
		//use binary search
	} else {
		// use linear search
		for(int i = startIndex; i < v->size; ++i){
			if(searchfn(key,v->base + i * v->elementSize) == 0)
				return i;
		}
	}
	return -1;
}

void VectorSort(vector* v, VectorCompareFunction comparefn){
	// implement quick sort
}

void VectorMap(vector* v, VectorMapFunction mapfn, void* auxData){
	assert(v != 0);
	assert(mapfn != 0);
	for (int i = 0; i < v->size; ++i){
		mapfn(v->base + i * v->elementSize, auxData);
	}
}