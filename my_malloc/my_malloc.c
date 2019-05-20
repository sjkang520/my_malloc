#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "my_malloc.h"
#include <stdbool.h>
void* first_block=NULL;
unsigned long heapSize;
//查找第一个符合要求的block，找不到返回NULL
//last指针始终指向当前遍历的block
void printList()
{
	t_block b=first_block;
	while(b)
	{
		printf("b:%p, b->pre:%p, b-next:%p, b->free:%d, b->size:%lu, b->data:%p\n",b,b->pre,b->next,b->free,b->size,b->data);
		b=b->next;
	}
	return;
}
t_block find_blockFF(t_block *last,size_t size)
{
	t_block b=first_block;
	while(b&&!(b->free&&b->size>=size))
	{
		*last=b;
		b=b->next;
	}
	return b;
}
//找到数据区大小大于size且差值最小的块
t_block find_Minblock(t_block* last,size_t s)
{
	t_block b=first_block;
	t_block minBlock=NULL;
	size_t min=0;
	while(b)
	{
		if(b->free&&(b->size>=s))
		{
			if(!minBlock)
			{
				minBlock=b;
				min=b->size-s;
			}
			else
			{
				if(b->size-s<min)
				{
					minBlock=b;
					min=b->size-s;
				}
			}
		}	
		*last=b;
		b=b->next;
	}
	return minBlock;
}
//在堆上申请新的内存块
t_block extend_heap(t_block last,size_t s)
{
	t_block b=NULL;
	b=sbrk(0);
	if(sbrk(BLOCK_SIZE+s)==(void*)-1)//sbrk执行错误
		return NULL;
	b->size=s;
	b->next=NULL;
	if(last)
	{
		b->pre=last;
		last->next=b;
	}

	b->ptr=b->data;
	b->free=0;
	return b;
}
//分离块，当一个块的大小足以容纳被请求的大小加上一个新的块（至少是块大小+8）时将该块分裂出一个新的块
//b需要分裂的块，s请求的内存大小
void split_block(t_block b,size_t s)
{
	t_block new=NULL;//分裂出的新的块
	new=b->data+s;//新块的地址
	new->size=b->size-s-BLOCK_SIZE;
	b->size=s;//分裂以后该块的大小
	new->next=b->next;
	if(new->next)
		new->next->pre=new;
	b->next=new;
	new->pre=b;
	new->free=1;

	new->ptr=new->data;
	b->ptr=b->data;
}
//合并两个连续空闲块，处理内存碎片
//b 前一个空闲块
t_block merge_block(t_block b)
{
	if(b->next&&b->next->free)
	{
		b->size+=b->next->size+BLOCK_SIZE;
		b->next=b->next->next;
		if(b->next)
			b->next->pre=b;
	}
	return b;
}
//malloc实现
//first fit:首次适应，从头开始找到第一个数据区大于要求size的块作为此次分配的块
void* ff_malloc(size_t s)
{
	t_block b=NULL;
	t_block last=NULL;
	size_t size=align8(s);
	if(first_block)
	{
		last=first_block;
		b=find_blockFF(&last,size);//查找合适的block
		if(b)
		{
			if(b->size>=size+BLOCK_SIZE+8)
			{
				split_block(b,size);
			}
			b->free=0;
		}
		//若没有找到合适的block，开辟新的块
		else
		{
			b=extend_heap(last,size);
			if(!b)
				return NULL;
		}
	}
	else
	{
		b=extend_heap(last,size);
		if(!b)
			return NULL;
		first_block=b;
	}
	return b->data;
}

//Best fit:从头开始遍历所有块，使用数据区大小大于size且差值最小的块作为此次分配的块
void* bf_malloc(size_t s)
{
	t_block b=NULL;
	t_block last=NULL;
	size_t size=align8(s);
	if(first_block)
	{
		last=first_block;
		b=find_Minblock(&last,size);
		if(b)
		{
			if(b->size>=size+BLOCK_SIZE+8)
				split_block(b,size);
			b->free=0;
		}
		else
		{
			b=extend_heap(last,size);
			if(!b)
				return NULL;
		}
	}
	else
	{
		b=extend_heap(last,size);
		if(!b)
			return NULL;
		first_block=b;
	}
	return b->data;
}

t_block getBlock(void* p)
{
	char* tmp=p;	
	return (p=tmp-=BLOCK_SIZE);	
}
//合法性检验
bool isValid_addr(void* p)
{
//	printf("p:%p,getBlock(p):%p\n",p,getBlock(p)->ptr);
	if(first_block)
	{
		if(p>first_block&&p<sbrk(0))
			return p==(getBlock(p)->ptr);
	}
	return 0;
}
//free之前进行合法性验证，保证要free的内存是我们malloc的地址
//方法：在结构体中添加一个指向第一个数据区的指针ptr，free之前比较
//参数指针是否等于ptr成员（即数据区首地址），相同可以说是合法的
void ff_free(void* p)
{
	if(isValid_addr(p))
	{
		t_block b=getBlock(p);
		b->free=1;
		if(b->pre&&b->pre->free)
		{
			b=merge_block(b->pre);//整合前一个内存块
		}
		if(b->next)
			b=merge_block(b);//整合后一个内存块
		else
		{
			//该内存块是在末尾
			if(b->pre)
				b->pre->next=NULL;
			else
				first_block=NULL;//该内存块唯一
			brk(b);
		}
	}
}
void bf_free(void* p)
{
	ff_free(p);
}
/*
unsigned long get_heapSize()
{
	unsigned long size=0;
//	printf("first_block:%p, sbrk(0):%p\n",first_block,sbrk(0));
	if(first_block)
		size=sbrk(0)-first_block;
	return size;
}
unsigned long get_free_heapSize()
{
	unsigned long size=0;
	t_block b=first_block;
	while(b)
	{
		if(b->free)
			size+=b->size;
		b=b->next;
	}
	return size;
}*/