#ifndef __WPMPROVIDERSINGLETON_H_INCLUDED__
#define __WPMPROVIDERSINGLETON_H_INCLUDED__

#include "FContext.h"
#include "IWorkPackageProvider.h"
#include "IQueue.h"
#include "WorkPackage.h"
#include "PackageQueue.h"
#include "WorkPackageManager.h"
#include "IWorkPackageManagement.h"

class WPManagerProviderSingleton {
public:
	static WPManagerProviderSingleton& getInstance();
	WorkPackageManager& getWpmInstance(int core_id);
	WorkPackageManager wpmArray[__MAX_WPM_CORES];
	WorkPackage getPackage(bool *queueStatus);
	bool addPackage(const WorkPackage&, bool *status);
private:
	WPManagerProviderSingleton() {
	};
	WPManagerProviderSingleton(WPManagerProviderSingleton const&) = delete;
	void operator=(WPManagerProviderSingleton const&) = delete;
};
#endif
