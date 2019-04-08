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
char word[5] = {'A', 'B', 'C', 'D', 'E'}; // program name

int *RR_queue;
int RR_size;
int RR_head;
int RR_tail;

int *MLFQ_queue[3];
int MLFQ_size[3];
int MLFQ_head[3];
int MLFQ_tail[3];
int MLFQ_timequantum[3] ={1,2,4};
int MLFQ_count[3] ={0, }; // current running process in Queue


void MLFQ_Queue(int _size)
{
	for(int i=0; i<3; i++)
	{
		MLFQ_size[i] = _size;
		MLFQ_queue[i] = (int*)malloc(sizeof(int) *MLFQ_size[i]);
		MLFQ_head[i] = 0;
		MLFQ_tail[i] = 0;
	}
}
bool MLFQ_InsertQ(int data, int Qnum)
{
	if((MLFQ_tail[Qnum]+1)%MLFQ_size[Qnum] == MLFQ_head[Qnum])
		return false;
	MLFQ_queue[Qnum][MLFQ_tail[Qnum]] = data;
	MLFQ_tail[Qnum] = (MLFQ_tail[Qnum] +1) % MLFQ_size[Qnum];
	MLFQ_count[Qnum]++;
	return true;
}
int MLFQ_DelQ(int Qnum)
{
	if(MLFQ_head[Qnum] == MLFQ_tail[Qnum])
		return -999;
	int temp;
	temp = MLFQ_queue[Qnum][MLFQ_head[Qnum]];
	MLFQ_head[Qnum] = (MLFQ_head[Qnum]+1)%MLFQ_size[Qnum];
	MLFQ_count[Qnum]--;
	return temp;
}

void Queue(int _size)
{
	RR_size = _size;
	RR_queue = (int*)malloc(sizeof(int) *RR_size);
	RR_head = 0;
	RR_tail = 0;
}
bool InsertQ(int data)
{
	if((RR_tail+1)%RR_size == RR_head)
		return false;

	RR_queue[RR_tail] = data;
	RR_tail = (RR_tail+1)%RR_size;
	return true;
}
int DelQ()
{
	if(RR_head==RR_tail)
		return -999;

	int temp;
	temp = RR_queue[RR_head];
	RR_head = (RR_head+1)%RR_size;
	return temp;
}

int* RR()
{
	int run_time[5] = {3, 6, 4, 5, 2}; // program run_time
	int arrival_time[5] = {0, 2, 4, 6, 8}; // program arrival_time
	bool Insert_program[5] = {false, }; // first insert bool
	int time_slice = 4; // time_slice
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
	// fill order
	for(int j=0; j<all_time; j=current_program_time)
	{
		int insert = DelQ();
		int work_time = 0;
		for(int k=current_program_time; k<current_program_time + time_slice; k++)
		{
			order[k] = insert;
			run_time[insert] = run_time[insert] - 1;
			work_time = work_time + 1;
			for(int i=0; i<sizeof(word); i++)
				if(!(Insert_program[i]) && arrival_time[i]<=k+1)
				{
					InsertQ(i);
					Insert_program[i]=true;
				}
			if(run_time[insert]==0)
				break;
		}
		if(run_time[insert] != 0)
			InsertQ(insert);
		current_program_time = current_program_time + work_time;
	}
	for(int i=0; i<all_time; i++)
		printf("%d ",order[i]);
	return order;
}

int* MLFQ()
{
        int run_time[5] = {3, 6, 4, 5, 2}; // program run_time
        int arrival_time[5] = {0, 2, 4, 6, 8}; // program arrival_time
        bool Insert_program[5] = {false, }; // first insert bool
        int time_slice = 4; // time_slice
        int current_time = 0; // current_time
        int all_time = 0; // add all run_time
        int * order; // save program number, order of time
        int current_program_time = 0;
	int count = 0; // current running process count in all Queue
	int current_queue = 0; // current running Queue

	// all runtime
        for(int i=0; i<sizeof(run_time)/sizeof(int); i++)
        {
                all_time = all_time + run_time[i];
        }
        order = (int*)malloc(sizeof(int)*all_time);
        MLFQ_Queue(10); // create size 10 Queue * 3
        // First insert program number in Queue0 (arrival_time == 0)
        for(int i=0; i<sizeof(word); i++)
        {       if(arrival_time[i]==0)
                {
                        MLFQ_InsertQ(i,0);
                        Insert_program[i] = true;
                }
        }

        // fill order
        for(int j=0; j<all_time; j=current_program_time)
        {
                int insert = MLFQ_DelQ(0);
		time_slice = MLFQ_timequantum[0];
		current_queue = 0;
		if(insert == -999)
		{
			insert = MLFQ_DelQ(1);
			time_slice = MLFQ_timequantum[1];
			current_queue = 1;
			if(insert == -999)
			{
				insert = MLFQ_DelQ(2);
				time_slice = MLFQ_timequantum[2];
				current_queue = 2;
			}	
		}
                int work_time = 0;
                for(int k=current_program_time; k<current_program_time + time_slice; k++)
                {
                        order[k] = insert;
                        run_time[insert] = run_time[insert] - 1;
                        work_time = work_time + 1;
                        for(int i=0; i<sizeof(word); i++)
                                if(!(Insert_program[i]) && arrival_time[i]<=k+1)
                                {
                                        MLFQ_InsertQ(i,0);
                                        Insert_program[i]=true;
                                }
                        if(run_time[insert]==0)
                                break;
                }
		for(int i=0; i<sizeof(MLFQ_count)/sizeof(int); i++)
		{
			count = count + MLFQ_count[i];
		}
                if(run_time[insert] != 0)
			if(count == 0)
        	                MLFQ_InsertQ(insert,0);
			else
			{
				if(current_queue+1<sizeof(MLFQ_count)/sizeof(int))
					MLFQ_InsertQ(insert, current_queue+1);
				else
					MLFQ_InsertQ(insert, current_queue);
			}
                current_program_time = current_program_time + work_time;
        }
        for(int i=0; i<all_time; i++)
                printf("%d ",order[i]);
	return order;
}

void print(int *order)
{
	for(int i=0; i<sizeof(order)/sizeof(int); i++)
	{
		
	}
	printf("name    0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 \n");
	for(int i=0; i<sizeof(word); i++)
	{
		printf("%4s    ",word[i]);
		for(int j=0; j<sizeof(order)/sizeof(int); j++)
		{
			if(word[i]==order[j])
				printf("
		}
		printf("\n");
	}
}




