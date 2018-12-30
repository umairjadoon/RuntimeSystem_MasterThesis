#ifndef __IWorkPackageProvider_H_INCLUDED__
#define __IWorkPackageProvider_H_INCLUDED__
#include "../include/WorkPackage.h"

class WorkPackageProvider{
public:
	virtual WorkPackage GetWorkPackage(bool *status) = 0;
	virtual WorkPackage stealPackage(bool *status) = 0;
	virtual ~WorkPackageProvider() {}

};


#endif
