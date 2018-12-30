#ifndef  __PROVIDEDFUNCTIONS_H_INCLUDED__
#define  __PROVIDEDFUNCTIONS_H_INCLUDED__
#include "Handler.h"

void workerThread(int thread_id);
bool createUserWorkPackage(void (*action)(void*), void* arguments, Handler** packagefinished);
void wait(Handler* handle);
void yield();

void print_context(const char*, void*);
int get_core();
int debug_printf(int level, const char* fmt, ...);


#endif
