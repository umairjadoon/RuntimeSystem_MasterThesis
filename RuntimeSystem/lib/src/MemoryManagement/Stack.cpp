/*
 * Stack.cpp
 *
 *  Created on: Sep 13, 2018
 *      Author: khm31335
 */

#include "Stack.h"
#include "MemoryPool.h"
#include "WorkPackageManager.h"
#include "WPMProviderSingleton.h"
#include "../../../HAL-Scheduling/include/corenumber.h"


Stack::Stack() {}
Stack::~Stack() {}

void* Stack::make_stack(){
	int core_id = coreNumber::getInstance()->getCoreID();
	WPManagerProviderSingleton& singletonObj =
					WPManagerProviderSingleton::getInstance();
		WorkPackageManager &wpm = singletonObj.getWpmInstance(core_id);
		return wpm.wpmMemPool.Allocate();

}
void Stack::release(void * stack ){
	int core_id= coreNumber::getInstance()->getCoreID();
	WPManagerProviderSingleton& singletonObj =
					WPManagerProviderSingleton::getInstance();
		WorkPackageManager &wpm = singletonObj.getWpmInstance(core_id);
		wpm.wpmMemPool.DeAllocate(stack);
}
