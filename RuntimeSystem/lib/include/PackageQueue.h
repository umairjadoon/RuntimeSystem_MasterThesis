#ifndef __PackageQueue_H_INCLUDED__
#define __PackageQueue_H_INCLUDED__

#include "IQueue.h"
#include "WorkPackage.h"
#include "IWorkPackageProvider.h"

class PackageQueue: public IQueue, public WorkPackageProvider {
public:
	int rear, front;
	int size;
	bool status;
	WorkPackage WorkPackageQueue[100];
	PackageQueue() {
		front = -1;
		rear = -1;
		size = sizeof(WorkPackageQueue) / sizeof(WorkPackageQueue[0]);
	}
 int count=0;
	virtual ~PackageQueue() {}
	int isFull();
	int isEmpty();
	WorkPackage GetWorkPackage(bool *status) final;
	WorkPackage stealPackage(bool *status)final;
	bool enqueueWorkPackage(WorkPackage workpackage, bool *status, int core_id) final;
};

#endif
