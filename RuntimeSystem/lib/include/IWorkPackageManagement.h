#ifndef __IWorkPackageManagement_H_INCLUDED__
#define __IWorkPackageManagement_H_INCLUDED__

#include "Handler.h"


class WorkPackageManagement {
public:
	virtual bool createWorkPackage(void (*action)(void*), void* arguments, bool* queueStatus, Handler** packagefinished) = 0;
	virtual ~WorkPackageManagement() {}
};


#endif
