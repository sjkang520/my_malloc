#include "my_malloc.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char* argv[])
{
	printf("debug\n");
	int *ptr=(int*)ff_malloc(2*sizeof(int));
	printf("sizeof of int:%ld\n",sizeof(int));
	*ptr=10;
	*(ptr+1)=11;
	printf("address=%p,\nvalue at ptr[0]:%d\n,value at ptr[1]:%d\n",ptr,*ptr,*(ptr+1));
	ff_free(ptr);
}