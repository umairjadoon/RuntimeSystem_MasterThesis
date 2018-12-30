#ifndef __FCONTEXT_H_INCLUDED__
#define __FCONTEXT_H_INCLUDED__
#include "context.h"
#include <pthread.h>

#define __MAX_WPM_CORES 3

class FContext {
	fcontext_data_t m_sched;	// storage for context of main process
	fcontext_t p_from, p_to;	// info from last jump
public:
	FContext();
	bool setJumpInfo(fcontext_t from, fcontext_t to);
	fcontext_t getSched();
	fcontext_t getCurr();
	fcontext_t getPrev();
	bool isSched();			// check if last "to" equals to "&m_sched"
};

class StealCounter {
        pthread_mutex_t lock;
        int counter;
        bool cores[__MAX_WPM_CORES];
public:
	StealCounter();
	~StealCounter();
        bool set(int core_id);
        bool reset(int core_id);
        int get();
};

extern StealCounter _stealCounter;
#endif
