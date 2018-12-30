#include <iostream>
#include<stdio.h>
#include <assert.h>

#include "FContext.h"
#include "WorkPackageManager.h"
#include "IQueue.h"
#include "PackageQueue.h"
#include "WorkPackage.h"
#include "context.h"
#include "Handler.h"
#include "WPMProviderSingleton.h"
#include "../../../UserDemoApp/startworkpackage.h"
#include "../../../HAL-Scheduling/include/corenumber.h"
#include "providedFunctions.h"

bool isStealCounterInc = false;
pthread_mutex_t exe_lock = PTHREAD_MUTEX_INITIALIZER;
StealCounter _stealCounter;

#ifdef __SHARED_MEMORY_POOL
MemoryPool WorkPackageManager::wpmMemPool;
#endif

bool WorkPackageManager::createWorkPackage(void (*action)(void*),
		void* arguments, bool* queueStatus, Handler** packagefinished) {
	WPManagerProviderSingleton& singletonObj =
			WPManagerProviderSingleton::getInstance();
	WorkPackage wpack(action, arguments, packagefinished);
	bool workPackageEnquedResult = singletonObj.addPackage(wpack, queueStatus);
	return workPackageEnquedResult;
}

bool createUserWorkPackage(void (*action)(void*), void* args,
		Handler** packagefinished) {
	bool status = true;
	bool *queueStatus = &status;
	WPManagerProviderSingleton& singletonObj =
			WPManagerProviderSingleton::getInstance();
	WorkPackage wpack(action, args, packagefinished);
	bool addpackage = singletonObj.addPackage(wpack, queueStatus);
	if (addpackage == false) {
		return false;
	} else {
		return true;
	}

}
void wait(Handler *packagefinished) {
	// check if wait needed
	if (packagefinished == nullptr || packagefinished->getHandle()) {
		return;
	}
	packagefinished->lock(); // mark handle as used
	// get reference to the current WPM
	WPManagerProviderSingleton& singletonObj =
			WPManagerProviderSingleton::getInstance();
	int core_id = coreNumber::getInstance()->getCoreID();
	WorkPackageManager &wpm = singletonObj.getWpmInstance(core_id);
	WorkPackage* wp = wpm.executionPackage;
	wp->m_waitfor = packagefinished;
	// prepare to the switch context and jump to the second part of the execute()
	fcontext_t curr_context = wpm.perThreadFctx.getCurr();
	fcontext_t next_context = wpm.perThreadFctx.getSched();
	if (wpm.perThreadFctx.setJumpInfo(curr_context, next_context)) {
		wpm.executionPackage->setState(blocked);
		// do jump and context switch
		jump_fcontext_1(curr_context, next_context, &wpm);
		// here is the second part of the yield() function
		// program returns here from the second and next calls of execute for the same package
		// a pointer to the current WPM as a return value from jump_fcontext() is received
	} else {
		// do nothing
	}
	packagefinished->unlock(); // mark handle as unused
}

void yield() {
	WPManagerProviderSingleton& singletonObj =
			WPManagerProviderSingleton::getInstance();
	int core_id = coreNumber::getInstance()->getCoreID();
	WorkPackageManager &wpm = singletonObj.getWpmInstance(core_id);
	// prepare to the switch context and jump to the second part of the execute()
	fcontext_t curr_context = wpm.perThreadFctx.getCurr();
	fcontext_t next_context = wpm.perThreadFctx.getSched();
	if (wpm.perThreadFctx.setJumpInfo(curr_context, next_context)) {
		wpm.executionPackage->setState(queued);
		// do jump and context switch
		jump_fcontext_1(curr_context, next_context, &wpm);
	} else {
		// do nothing

		return;
	}
}

void wait_for_all(bool* queueStatus) {
	while (true) {
		WPManagerProviderSingleton& singletonObj =
				WPManagerProviderSingleton::getInstance();
		int core_id = coreNumber::getInstance()->getCoreID();
		WorkPackageManager &wpm = singletonObj.getWpmInstance(core_id);
		WorkPackage singleWorkPackage = singletonObj.getPackage(queueStatus);
		if (*queueStatus == false) {
			break;
		}
//		WorkPackage* wp = &singleWorkPackage;
		singleWorkPackage.execute(core_id);
		if (singleWorkPackage.handle->getHandle() == false) {
			bool status;
			bool is_ok = singletonObj.addPackage(*wpm.executionPackage,
					&status);
			assert(is_ok);

		}
	}
}

void workerThread(int thread_id) {
	WPManagerProviderSingleton& singletonObj =
			WPManagerProviderSingleton::getInstance();
	WorkPackageManager &wpm = singletonObj.getWpmInstance(thread_id);
	bool status = true;
	bool *queueStatus = &status;
	if (thread_id == 0) {
		Handler* packagefinished;
		bool result = wpm.createWorkPackage(initialWorkPackage, nullptr,
				queueStatus, &packagefinished);
		assert(result);

		while (_stealCounter.get() != __MAX_WPM_CORES) {
			wait_for_all(queueStatus);
		}

	} else {
		while (_stealCounter.get() != __MAX_WPM_CORES) {
			WorkPackage otherCoreWp = singletonObj.getPackage(queueStatus);
			if (*queueStatus == true) {
				wpm.executionPackage = &otherCoreWp;
				WorkPackage* wp = &otherCoreWp;
				otherCoreWp.execute(thread_id);
				if (wp->handle->getHandle() == false) {
					bool status;
					if (!singletonObj.addPackage(*wp, &status)) {
						assert(false);
						exit(-3);
					}
				}
			}
		}
	}
}

