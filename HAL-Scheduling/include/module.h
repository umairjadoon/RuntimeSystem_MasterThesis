#ifndef _MODULE_H
#define _MODULE_H


void *startFunc(void *threadid);
struct args {
	int a;
	int b;
};
struct thread_data {
	   int  thread_id;
	};

#endif /*_MODULE_H*/
