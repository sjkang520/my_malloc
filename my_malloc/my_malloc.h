#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

#include <stddef.h>
#define align8(x) (((((x)-1)>>3)<<3)+8) //内存对齐，得到的此数是最接近x且比x大的8的倍数
#define BLOCK_SIZE 40
typedef struct s_block* t_block;//meta区的数据结构
struct s_block
 {
 	size_t size;//数据区域大小
 	t_block next;//指向下一区域的指针
 	t_block pre;//指向上一区域的指针
 	void* ptr;//指向data
 	int free;//是否为空闲区域
 	int padding;//填充4字节，内存对其，保证meta块长度为8的倍数
 	char data[1];//虚拟字段，表示数据块的第一个字节，不计入meta长度
 }; 
 extern void *ff_malloc(size_t s);//首次适应法
 extern void ff_free(void* p);
 extern void* bf_malloc(size_t s);//最佳适应法
 extern void bf_free(void* p);
// extern unsigned long get_heapSize();
 //extern unsigned long get_free_heapSize();
 extern void printList();
#endif