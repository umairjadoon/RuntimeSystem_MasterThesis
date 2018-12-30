#include <iostream>
#include <cstdlib>
#include<stdio.h>
#include <pthread.h>
#include "../include/module.h"
#include "../include/corenumber.h"

using namespace std;

int main(int argc, char **argv) {

	int noOfThreads = 0;
	if (argc > 1) {
		noOfThreads = atoi(argv[1]);
	} else {
		std::cout
				<< "Please give no of threads to the program as first argument!"
				<< std::endl;
		return 1;
	}

	coreNumber::getInstance()->set_values(noOfThreads);
	pthread_t thread_ids[noOfThreads];
	struct thread_data td[noOfThreads];
	int rc;
	int i;
	pthread_attr_t attr;
	void *status;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (i = 0; i < noOfThreads; i++) {
		td[i].thread_id = i;
		rc = pthread_create(&thread_ids[i], NULL, startFunc, &td[i]);
		coreNumber::getInstance()->setCoreID(thread_ids[i], td[i].thread_id);
		if (rc != 0) {
			cout << "Error:unable to create thread," << rc << endl;
			exit(-1);
		}

	}
	pthread_attr_destroy(&attr);
	for (i = 0; i < noOfThreads; i++) {
		rc = pthread_join(thread_ids[i], &status);
		if (rc) {
			cout << "Error:unable to join," << rc << endl;
			exit(-1);
		}
	}

//	cout << "Main thread is exiting." << endl;
	pthread_exit(NULL);
}
