#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include"PackageQueue.h"
#include "WorkPackage.h"
#include "WorkPackageState.h"
#include "IQueue.h"
#include "providedFunctions.h"


pthread_mutex_t getlock = PTHREAD_MUTEX_INITIALIZER;

int PackageQueue::isFull() {
	if ((rear == 0 && front == size - 1) || (front == rear - 1))
		return 1;
	return 0;
}

int PackageQueue::isEmpty() {
	if (rear == -1)
		return 1;
	return 0;
}

bool PackageQueue::enqueueWorkPackage(WorkPackage wpackage,
		bool *notemptystatus, int core_id) {
	pthread_mutex_lock(&getlock);

	if (isFull()) {
		printf("Queue is full for Core %d \n", core_id);
		pthread_mutex_unlock(&getlock);
		assert(false);
	}

	if (rear == -1) { /* Insert First Element */
		rear = front = 0;
		WorkPackageQueue[front] = wpackage;
		//front++;
	} else if (front == size - 1 && rear != 0) {
		front = 0;
		WorkPackageQueue[front] = wpackage;
	} else {
		front++;
		WorkPackageQueue[front] = wpackage;

	}
	_stealCounter.reset(core_id);
	*notemptystatus = true;
	pthread_mutex_unlock(&getlock);

	return true;

}

WorkPackage PackageQueue::GetWorkPackage(bool *queueStatus){
	pthread_mutex_lock(&getlock);

	if (isEmpty()) {
		*queueStatus = false;
		pthread_mutex_unlock(&getlock);
		return WorkPackage();
	}

	WorkPackage data = WorkPackageQueue[rear];

	if (front == rear)
		rear = front = -1;
	else if (rear == size - 1)
		rear = 0;
	else
		rear++;
	*queueStatus = true;
	pthread_mutex_unlock(&getlock);

	return data;
}

WorkPackage PackageQueue::stealPackage(bool *queueStatus) {
	pthread_mutex_lock(&getlock);

	if (isEmpty()) {
		*queueStatus = false;
		pthread_mutex_unlock(&getlock);
		return WorkPackage();
	}

	WorkPackage data=WorkPackageQueue[front];
	// if queue has only one element
	if (front == rear)
		rear = front = -1;
	else if (front == 0)
		front = size-1;
	else
		front--;
	*queueStatus = true;
	pthread_mutex_unlock(&getlock);

	return data;
}

