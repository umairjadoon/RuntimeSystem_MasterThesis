#ifndef LIB_SRC_WORKPACKAGEMANAGEMENT_HANDLER_H_
#define LIB_SRC_WORKPACKAGEMANAGEMENT_HANDLER_H_

class Handler {
private:
	int  next;		// index of the next free handler
	int  m_lockcount;	// number of packages waiting for this handle
	bool m_packagefinished; // if true then blocked package can be unblocked
	void setHandle();	// set m_packagefinished to true
	bool getHandle();	// get m_packagefinished
	void clearHandle();	// set m_packagefinished to false
	void lock();		// function called when handle is used
	void unlock();		// function called when handle is not needed anymore

public:
	int id; // handler id
	Handler();
	virtual ~Handler();
	friend class WorkPackage;
	friend class WorkPackageManager;
	friend void wait(Handler *packagefinished);
	friend void wait_for_all(bool* statusQueue);
	friend void workerThread(int thread_id);
	friend bool createUserWorkPackage(void (*action)(void*), void* args, Handler** packagefinished); 
	friend Handler* getHandler(int id);
	friend class WPManagerProviderSingleton;
	friend class PackageQueue;
};

Handler* getHandler(int id);

#endif /* LIB_SRC_WORKPACKAGEMANAGEMENT_HANDLER_H_ */
