#include "my_malloc.h"
#include <stdio.h>
//#define _BF

#ifdef _BF
#define MALLOC(s) bf_malloc(s)
#define FREE(p)   bf_free(p)
#else
#define MALLOC(s) ff_malloc(s)
#define FREE(p)   ff_free(p)
#endif 

int main(int argc,char* argv[])
{
	const int num=8;
	int sum=0;
	int size;
	int *array[num];
	size=4;
	array[0]=(int*)MALLOC(size*sizeof(int));
	for(int i=0;i<size;i++)
		array[0][i]=4;
	for(int i=0;i<size;i++)
		sum+=array[0][i];

	size=8;
	array[1]=(int*)MALLOC(size*sizeof(int));
	for(int i=0;i<size;i++)
		array[1][i]=4;
	for(int i=0;i<size;i++)
		sum+=array[1][i];

	size=16;
	array[2]=(int*)MALLOC(size*sizeof(int));
	for(int i=0;i<size;i++)
		array[2][i]=4;
	for(int i=0;i<size;i++)
		sum+=array[2][i];

	size=32;
	array[3]=(int*)MALLOC(size*sizeof(int));
	for(int i=0;i<size;i++)
		array[3][i]=4;
	for(int i=0;i<size;i++)
		sum+=array[3][i];

	size=64;
	array[4]=(int*)MALLOC(size*sizeof(int));
	for(int i=0;i<size;i++)
		array[4][i]=4;
	for(int i=0;i<size;i++)
		sum+=array[4][i];

	size=128;
	array[5]=(int*)MALLOC(size*sizeof(int));
	for(int i=0;i<size;i++)
		array[5][i]=4;
	for(int i=0;i<size;i++)
		sum+=array[5][i];

	
	size=256;
	array[6]=(int*)MALLOC(size*sizeof(int));
	for(int i=0;i<size;i++)
		array[6][i]=4;
	for(int i=0;i<size;i++)
		sum+=array[6][i];

	FREE(array[1]);
	FREE(array[3]);
	FREE(array[5]);

	printf("after free 1,3,5,list is:\n");
    printList();

	size=33;
	array[7]=(int*)MALLOC(size*sizeof(int));
	for(int i=0;i<size;i++)
		array[7][i]=4;
	for(int i=0;i<size;i++)
		sum+=array[7][i];

	printf("malloc array[7],list is:\n");
	printList();
	
	FREE(array[0]);
	FREE(array[2]);
	FREE(array[4]);
	FREE(array[6]);
	FREE(array[7]);

//	printf("finally list is:\n");
	printList();
//	printf("end\n");
    return 0;
}