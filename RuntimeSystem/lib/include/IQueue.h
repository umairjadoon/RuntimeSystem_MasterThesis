#ifndef __IQueue_H_INCLUDED__
#define __IQueue_H_INCLUDED__

//Header Files
#include "WorkPackage.h"

class IQueue{
public:
	virtual bool enqueueWorkPackage(WorkPackage workpackage, bool *status, int core_id) = 0;
	virtual ~IQueue() {}
};


#endif
