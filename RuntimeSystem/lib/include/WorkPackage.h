#ifndef __WorkPackage_H_INCLUDED__
#define __WorkPackage_H_INCLUDED__

#include "FContext.h"
#include "WorkPackageState.h"
#include "Stack.h"
#include "MemoryPool.h"
#include "../../../HAL-Scheduling/include/corenumber.h"
#include "Handler.h"

class WorkPackage {
private:
	void (*m_action)(void*);
	void* m_arguments = nullptr;
protected:
	static int id;

public:
	WorkPackageState wp_state;
	WorkPackage(){};
	WorkPackage(void (*action)(void*), void* arguments, Handler** packagefinished);
	void destroystack();
	void execute(int id);
	void setState(WorkPackageState wp_state);
	WorkPackageState getState();
	static void run_wrapper(void* vp) noexcept;
	Stack Wp_localstack;
	fcontext_t m_context;
	int packageId;
	Handler* handle;
	Handler* m_waitfor;
};


#endif
