/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32151574 
*	    Student name : Moon Hyun Ho
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
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
typedef enum {false, true} bool;
/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 */
int *queue;
int size;
int head;
int tail;

void Queue(int _size)
{
	size = _size;
	queue = (int*)malloc(sizeof(int) *size);
	head = 0;
	tail = 0;
}
bool InsertQ(int data)
{
	if((tail+1)%size == head)
		return false;

	queue[tail] = data;
	tail = (tail+1)%size;
	return true;
}
int DelQ()
{
	if(head==tail)
		return -999;

	int temp;
	temp = queue[head];
	head = (head+1)%size;
	return temp;
}

void RR()
{
	char word[3] = {'A', 'B', 'C'}; // program name
	int run_time[3] = {5, 5, 5}; // program run_time
	int arrival_time[3] = {0, 0, 0}; // program arrival_time
	bool Insert_program[3] = {false, false, false}; // first insert bool
	int time_slice = 1; // time_slice
	int current_time = 0; // current_time
	int all_time = 0; // add all run_time
	int * order; // save program number, order of time
	int current_program_time = 0; 

	// all runtime
	for(int i=0; i<sizeof(run_time)/sizeof(int); i++)
	{
		all_time = all_time + run_time[i];
	}
	order = (int*)malloc(sizeof(int)*all_time);
	Queue(10); // create size 10 Queue
	// First insert program number in Queue(arrival_time == 0)
	for(int i=0; i<sizeof(word); i++)
	{	if(arrival_time[i]==0)
		{
			InsertQ(i);
			Insert_program[i] = true;
		}
	}
	for(int j=0; j<sizeof(order)/sizeof(int); j=current_program_time)
	{
		int insert = DelQ();
		for(int k=current_program_time; k<current_program_time + time_slice; k++)
		{
			order[k] = insert;
			run_time[insert] = run_time[insert] - 1;
			
			for(int i=0; i<sizeof(word); i++)
				if(!(Insert_program[i]) && arrival_time[i]<=k)
				{
					InsertQ(i);
					Insert_program[i]=true;
				}
		}
		if(run_time[insert] != 0)
			InsertQ(insert);
		current_program_time = current_program_time + time_slice;
	}
	for(int i=0; i<sizeof(order)/sizeof(int); i++)
		printf("%d ",order[i]);
}



