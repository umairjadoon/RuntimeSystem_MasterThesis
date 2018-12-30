#include "Handler.h"
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "WorkPackage.h"

pthread_mutex_t handle_lock = PTHREAD_MUTEX_INITIALIZER;
int static last_id = 0;

Handler handlerPool[64];

static int s_init = 0; // Number of initialized handles in pool
static int s_next = 0; // index of next free handle
static int s_free = int(sizeof(handlerPool)/sizeof(handlerPool[0])); // no of free handles
static int s_tnum = int(sizeof(handlerPool)/sizeof(handlerPool[0])); // total no of handles
static int s_used = 0;

Handler::Handler() {
        id = ++last_id;
        m_lockcount = 0;
}

Handler::~Handler() {
}

//Set the value to false
void Handler::clearHandle(){
        pthread_mutex_lock(&handle_lock);
        m_packagefinished=false;
        pthread_mutex_unlock(&handle_lock);
}

//Set the value of packagefinished to true when execution is finished
void Handler::setHandle(){
        pthread_mutex_lock(&handle_lock);
        m_packagefinished=true;;
        pthread_mutex_unlock(&handle_lock);
}
//Get the value of handle inside the wait function and act accordingly.
bool Handler::getHandle(){
        pthread_mutex_lock(&handle_lock);
        bool packagestate= m_packagefinished;
        pthread_mutex_unlock(&handle_lock);
        return packagestate;
}

void Handler::lock() {
        pthread_mutex_lock(&handle_lock);
        m_lockcount++;
        pthread_mutex_unlock(&handle_lock);
}


void Handler::unlock() {
        pthread_mutex_lock(&handle_lock);
        if (--m_lockcount == 0 && m_packagefinished) {
                next = s_next;
                s_next = this - handlerPool;
		s_free++;
        }
        pthread_mutex_unlock(&handle_lock);
}

Handler* getHandler(int id) {
        Handler* ret = nullptr;
        pthread_mutex_lock(&handle_lock);
        if (s_init < s_tnum) {
                handlerPool[s_init].next = s_init + 1;
		s_init++;
        }

        assert(s_free > 0);

        ret = handlerPool+s_next;
        s_next = ret->next;
        ret->id = id;
        s_free--;
        pthread_mutex_unlock(&handle_lock);
	if (s_tnum - s_free > s_used) s_used = s_tnum - s_free;
        return ret;
}
