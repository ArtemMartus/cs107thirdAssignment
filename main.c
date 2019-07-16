#include "vector.h"
#include <stdio.h> // printf
#include <assert.h> // assert
#include <stdlib.h> // malloc, free



int compareInts(const void* a, const void* b){
	int i1 = *(int*)a;
	int i2 = *(int*)b;
	return i2 - i1;
}

void mapInts(void* pElement,void* pbyNum){ // add function
	int* element = (int*)pElement;
	int byNum = *(int*)pbyNum;
	*element += byNum; 
}

void printPosition(vector* v,int q,int w,int e,int r,bool isSorted){

	int a = VectorSearch(v,&q,compareInts,0,isSorted),
		b = VectorSearch(v,&w,compareInts,0,isSorted),
		c = VectorSearch(v,&e,compareInts,0,isSorted),
		d = VectorSearch(v,&r,compareInts,0,isSorted);

	printf("Positions of values { %d %d %d %d } in vector { %d %d %d %d }\n",q,w,e,r,a,b,c,d);
}

void printVector(vector* v){
	void* pos0 = VectorNth(v,0); assert(pos0 != 0);
	void* pos1 = VectorNth(v,1); assert(pos1 != 0);
	void* pos2 = VectorNth(v,2); assert(pos2 != 0);
	void* pos3 = VectorNth(v,3); assert(pos3 != 0);

	int a = *(int*)pos0,
		b = *(int*)pos1,
		c = *(int*)pos2,
		d = *(int*)pos3;
	printf("Values from vector %d %d %d %d\n",a,b,c,d);
}


int main(){
	printf("Hello, World!\n");
	vector* v = malloc(sizeof(vector)); //vector of integers
	VectorNew(v,sizeof(int),0,3); 
// lets fill this vector with {2,0,1,9}
	int number;
	number = 2;VectorAppend(v,(void*)&number);
	number = 0;VectorAppend(v,(void*)&number);
	number = 1;VectorAppend(v,(void*)&number);
	number = 9;VectorAppend(v,(void*)&number);

	printVector(v);

//Replace to 1998
	number = 1;VectorReplace(v,&number,0);
	number = 9;VectorReplace(v,&number,1);
	number = 9;VectorReplace(v,&number,2);
	number = 8;VectorReplace(v,&number,3);

	printVector(v);

//Remove and insert element to form 1787
	VectorDelete(v,1);
	VectorDelete(v,1);
	number = 7;VectorInsert(v,&number,1);
	VectorInsert(v,&number,3); 

	printVector(v);

//Search for an element
	printPosition(v,1,7,8,7,false);

//Sort the vector
	VectorSort(v,compareInts);

	printVector(v);
//Binary search the element
	printPosition(v,1,7,8,7,true);

//Use vector mapping
	number = 3;VectorMap(v,mapInts,(void*)&number);

	printVector(v);

//Free memory
	VectorDispose(v);
	free(v);

	return 0;
}