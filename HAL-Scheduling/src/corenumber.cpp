#include "../include/corenumber.h"
#include "../include/module.h"
#include <iostream>
#include <map>

coreNumber* coreNumber::instance;

int coreNumber::getCoreCount() {
	return core_count;
}
void coreNumber::setCoreID(pthread_t pid, int id ){
	thread_ids[pid]=id;
}

int coreNumber::getCoreID() {
	return thread_ids[pthread_self()];
}
