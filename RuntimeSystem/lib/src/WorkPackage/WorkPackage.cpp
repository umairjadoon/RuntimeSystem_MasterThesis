#include <iostream>
#include<stdio.h>
#include <assert.h>

#include "WorkPackage.h"
#include "WorkPackageState.h"
#include "context.h"
#include "Stack.h"
#include "WorkPackageManager.h"
#include "WPMProviderSingleton.h"

int WorkPackage::id=0;

WorkPackage::WorkPackage(void (*action)(void*), void* arguments, Handler** packagefinished) {
		Wp_localstack.local_stack= Stack::make_stack(); //Local stack of each thread for the purpose of context switching
		m_action = action; // function associated with every thread that its going to execute
		m_arguments = arguments;//Arguments passed to the execution function
		m_context = make_fcontext_1(Wp_localstack.local_stack, 0x10000, run_wrapper);
		wp_state = queued; // Initial state of the package as it is created.
		packageId=++id;
		handle = getHandler(packageId);
		if (packagefinished) {
			*packagefinished = handle;
		}
		handle->id = packageId;
		handle->clearHandle();
		m_waitfor = nullptr;
}


void WorkPackage::execute(int thread_id) {
	WPManagerProviderSingleton& singletonObj = WPManagerProviderSingleton::getInstance();
	WorkPackageManager &wpm = singletonObj.getWpmInstance(thread_id);
	wpm.executionPackage = this;
	if (wp_state == blocked) {
		// check if package can be unblocked
		if (m_waitfor != nullptr && !m_waitfor->getHandle()) {
			// no it cannot be unblocked
			// it will be returned to the queue
			return;
		}
		else {
			// yes it can be unblocked
			wp_state = queued;
		}
	}

	if (wp_state == finished || handle->getHandle()) {
		//do nothing for finished packages
		// just return
		return;
	}

	if (wpm.perThreadFctx.isSched()) {
		// the call is from the scheduler context
		fcontext_t current_context = wpm.perThreadFctx.getSched();
		// save from and to context
		if (wpm.perThreadFctx.setJumpInfo(current_context, m_context))
		{
			// mark package as running
			setState(running);
			// switch context with jump to the run_wrapper or to the second part of the yield() or wait()
			// second part is a part after jump_fcontext() inside function
			jump_fcontext_1(current_context, m_context, &wpm);
			// this is the second part of execute. code jump here from the first part of yield() or wait()
			// or run_wrapper()
			}
		else {
			assert(false);
		}
	}
	else {
		printf("Error: calling execute from package context\n");
		assert(false);
	}

	if (wp_state == finished) {
		destroystack();
	}
}

void WorkPackage::setState(WorkPackageState state) {
	wp_state = state;
}

void WorkPackage::destroystack(){
	if (Wp_localstack.local_stack != nullptr) {
		 Stack::release(Wp_localstack.local_stack);
	}
}

void WorkPackage::run_wrapper(void* vp) noexcept
{
	WPManagerProviderSingleton& singletonObj = WPManagerProviderSingleton::getInstance();
	WorkPackageManager* arg = (WorkPackageManager*) vp;
    arg->executionPackage->m_action(arg->executionPackage->m_arguments);
	int core_id = coreNumber::getInstance()->getCoreID(); 
	WorkPackageManager &wpm = singletonObj.getWpmInstance(core_id);
	WorkPackage* wp = wpm.executionPackage;
	wp->setState(finished);
	wp->handle->setHandle();
	fcontext_t curr_context = wpm.perThreadFctx.getCurr();
	fcontext_t next_context = wpm.perThreadFctx.getSched();
	assert(curr_context != next_context); // run_wrapper should run in the package context
	if (wpm.perThreadFctx.setJumpInfo(curr_context, next_context)) {
	jump_fcontext_1(curr_context, next_context, &wpm);
	//  should never return to the finished package
	// but for the error detection will continue this code
	}
	assert(false); // attempt to execute a finished task
}

