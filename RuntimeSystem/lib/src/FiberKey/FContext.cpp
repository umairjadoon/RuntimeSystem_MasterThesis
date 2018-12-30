#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include "FContext.h"

FContext::FContext() {
	p_from = &m_sched;
	p_to   = &m_sched;
}

bool FContext::setJumpInfo(fcontext_t from, fcontext_t to)
{
	if (from == to)
		return false;
	p_from = from;
	p_to = to;
	return true;
}

fcontext_t FContext::getSched() {
	return (fcontext_t) &m_sched;
}

fcontext_t FContext::getCurr() {
	return p_to;
}

fcontext_t FContext::getPrev() {
	return p_from;
}

bool FContext::isSched() {
	return (p_to == (fcontext_t) &m_sched);
}

StealCounter::StealCounter() {
	lock = PTHREAD_MUTEX_INITIALIZER;
	for(int i = 0; i < __MAX_WPM_CORES; i++) {
		cores[i] = false;
	}
}

StealCounter::~StealCounter() {
	fflush(stdout);
}

bool StealCounter::set(int core_id) {
        bool ret = false;
        pthread_mutex_lock(&lock);
        if (!cores[core_id]) {
                cores[core_id] = true;
                counter++;
        }
        pthread_mutex_unlock(&lock);
        return ret;
}

bool StealCounter::reset(int core_id) {
        bool ret = false;
        pthread_mutex_lock(&lock);
        if (cores[core_id]) {
                cores[core_id] = false;
                counter--;
        }
        pthread_mutex_unlock(&lock);
        return ret;
}

int StealCounter::get() {
        int ret = false;
        pthread_mutex_lock(&lock);
        ret = counter;
        pthread_mutex_unlock(&lock);
        return ret;
}
pthread_mutex_t printlock = PTHREAD_MUTEX_INITIALIZER;
