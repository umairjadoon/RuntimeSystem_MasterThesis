#ifndef LIB_INCLUDE_STACK_H_
#define LIB_INCLUDE_STACK_H_

#include "MemoryPool.h"

class Stack {
public:
	void *local_stack =nullptr;
	Stack();
	 static void* make_stack();
	 static void release( void* stack );
	~Stack();
};

#endif /* LIB_INCLUDE_STACK_H_ */
