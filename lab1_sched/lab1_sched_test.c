/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32151574 
*	    Student name : Moon Hyun Ho
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

/*
 * you need to implement scheduler simlator test code.
 *
 */

int main(int argc, char *argv[]){
	int *order;
	printf("FIFO\n");
	order = FIFO();
	print(order);
	printf("RR\n");
	order = RR();
	print(order);
	printf("\n");
	printf("MLFQ\n");
	order = MLFQ();
	print(order);
	printf("Lottery\n");
	order = Lottery();
	print(Lottery());
}

