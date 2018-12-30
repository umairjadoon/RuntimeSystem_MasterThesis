#include <iostream>
#include "../include/module.h"
#include "../include/corenumber.h"
#include "providedFunctions.h"


	void *startFunc(void *threadarg) {
	   struct thread_data *my_data;
	   my_data = (struct thread_data *) threadarg;
//	   printf("Core %d has started \n ", my_data->thread_id);
	   workerThread(my_data->thread_id);
	   pthread_exit(NULL);
}
