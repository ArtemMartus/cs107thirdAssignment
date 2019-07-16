#include "vector.h"


int main(){
	printf("Hello, World!\n");
	vector* v = malloc(sizeof(vector)); //vector of integers
	VectorNew(v,sizeof(int),0,3); 
	VectorDispose(v);
	free(v);
	return 0;
}