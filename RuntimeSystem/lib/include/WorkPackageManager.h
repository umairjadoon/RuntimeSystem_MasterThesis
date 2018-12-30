#ifndef __WorkPackageManager_H_INCLUDED__
#define __WorkPackageManager_H_INCLUDED__

#include "IWorkPackageManagement.h"
#include "PackageQueue.h"
#include "MemoryPool.h"
#include "FContext.h"


void workerThread(int thread_id);

class WorkPackageManager: public WorkPackageManagement {
public:
	PackageQueue perThreadPQ;
#ifdef __SHARED_MEMORY_POOL
	static MemoryPool wpmMemPool;
#else
	MemoryPool wpmMemPool;
#endif
	FContext perThreadFctx;
	WorkPackage* executionPackage =nullptr;
	bool createWorkPackage(void (*action)(void*), void* arguments, bool* queueStatus, Handler** packagefinished) final;
	WorkPackageManager() {};
};

#endif
