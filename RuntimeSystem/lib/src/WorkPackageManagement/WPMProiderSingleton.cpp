#include <iostream>
#include<stdio.h>
#include <pthread.h>
#include <assert.h>

#include "FContext.h"
#include "IWorkPackageManagement.h"
#include "PackageQueue.h"
#include "IQueue.h"
#include "WorkPackage.h"
#include "WorkPackageManager.h"
#include "WorkPackageState.h"
#include "../../../HAL-Scheduling/include/corenumber.h"
#include "WPMProviderSingleton.h"


pthread_mutex_t lockCounter = PTHREAD_MUTEX_INITIALIZER;
WPManagerProviderSingleton& WPManagerProviderSingleton::getInstance() {
	static WPManagerProviderSingleton instance;
	return instance;
}

WorkPackageManager& WPManagerProviderSingleton::getWpmInstance(int core_id) {
	assert (core_id < __MAX_WPM_CORES);
	return wpmArray[core_id];
}
bool WPManagerProviderSingleton::addPackage(const WorkPackage& wpackage,
		bool *status) {
	int core_id = coreNumber::getInstance()->getCoreID();
	WorkPackageManager &wpm = wpmArray[core_id];

	bool result = wpm.perThreadPQ.enqueueWorkPackage(wpackage, status, core_id);
	if (result) _stealCounter.reset(core_id);
	return result;
}
WorkPackage WPManagerProviderSingleton::getPackage(bool *queueStatus)
{
	int core_id = coreNumber::getInstance()->getCoreID(); // @suppress("Function cannot be resolved") // @suppress("Method cannot be resolved")
	WorkPackageManager &wpm = wpmArray[core_id];
    WorkPackage singleWorkPackage = wpm.perThreadPQ.GetWorkPackage(queueStatus);
    //If local Queue is not empty return the retrieved package other wise steal the package
	if (*queueStatus == true) {
		return singleWorkPackage;
		}
	else
	{
		_stealCounter.set(core_id);
		int index= sizeof(wpmArray) / sizeof(wpmArray[0]);
		while(_stealCounter.get() !=index)
		{
			for (int i = 0; i < index; i++)
			{
				if (i != core_id)
				{
					int emptyqueue = wpmArray[i].perThreadPQ.isEmpty();
					if(emptyqueue==0){
						pthread_mutex_lock(&lockCounter);
					 	singleWorkPackage = wpmArray[i].perThreadPQ.stealPackage(queueStatus);
					 	pthread_mutex_unlock(&lockCounter);
						if(*queueStatus==true)
						{
							_stealCounter.reset(core_id);
							return singleWorkPackage;
						}
					}
				}
			}
		}
	}
	return WorkPackage();
}
