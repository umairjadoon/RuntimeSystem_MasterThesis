#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "startworkpackage.h"
#include "../RuntimeSystem/lib/include/providedFunctions.h"
#include "../RuntimeSystem/lib/include/Handler.h"

args user_arguments = {25,0};

#define number a
#define result b

void fibonacci (void* arguments)
{
     Handler* packagefinished;
     args* local_args = (args*) arguments;

     if (local_args->number < 2) {
         local_args->result= local_args->number;
     }
     else {
     	args local_args1 = {0,0};
     	args local_args2 = {0,0};
     	local_args1.number = local_args->number-1;
     	local_args2.number = local_args->number-2;
     	if (!createUserWorkPackage(fibonacci, &local_args1, &packagefinished)) {
		printf("UserPackage creation failed\n");
		assert(false);
	}
	wait(packagefinished);
     	fibonacci(&local_args2);
     	local_args->result = local_args1.result + local_args2.result;
     }
}

void print_odd (void* arguments)
{
    for(int i = 1; i < 10; i += 2)
    {
    printf("printing odd number :  %d\n", i);
	yield();
    }
}

void print_even(void* arguments)
{
    for(int i = 2; i < 10; i += 2)
    {
    printf("printing even number :  %d\n", i);
	yield();
    }
}

void initialWorkPackage(void*) {
	Handler* wait1; // use this Handler to wait for Work Package
	bool is_ok = createUserWorkPackage(fibonacci, &user_arguments, &wait1);
	assert(is_ok); // terminate in the case of problems with the package creation
	wait(wait1);
	printf("SUM OF FABONACCI(%d) SEQUENCE IS %d\n", user_arguments.number, user_arguments.result);

	//Yield function demonstration
	Handler* wait2;
	createUserWorkPackage(print_odd,  nullptr, &wait2);
	createUserWorkPackage(print_even, nullptr, nullptr);
	wait(wait2);
	printf("end of init\n");
}

