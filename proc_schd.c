#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "config.h"

void unit_time()
{
	volatile unsigned long i;
	for(i = 0; i < 1000000UL; i++);
	return ;
}

//FIFO
void schd_FIFO(Process *proc, int proc_num)
{
	for(int i = 0; i < proc_num; i++) {
		int pid, statloc;
		if((pid = fork()) == 0) {
			for(int j = 0; j < proc[i].exe_time; j++)
				unit_time();
			exit(1);
		}
		waitpid(pid, &statloc, 0);
		printf("%s %d\n", proc[i].name, pid);
	}
	return ;
}

int select_SJF(Process *proc, int endpoint)
{
	int min_time = 100000000;
	int index = -1;
	for(int i = 0; i < endpoint; i++) {
		if(proc[i].is_execed == 1) continue;
		if(proc[i].exe_time < min_time) {
			min_time = proc[i].exe_time;
			index = i;
		}
	}
	return index;
}

void schd_SJF(Process *proc, int proc_num)
{
	int now_time = proc[0].ready_time;
	int finished = 0;
	int endpoint = 0;
	while(finished < proc_num) {
		while(proc[endpoint].ready_time <= now_time && endpoint < proc_num) {
			endpoint++;
		}
		int index = select_SJF(proc, endpoint);
		int pid, statloc;
		if((pid = fork()) == 0) {
			for(int j = 0; j < proc[index].exe_time; j++)
				unit_time();
			exit(1);
		}
		waitpid(pid, &statloc, 0);
		printf("%s %d\n", proc[index].name, pid);
		proc[index].is_execed = 1;
		now_time += proc[index].exe_time;
		if(finished > endpoint && endpoint < proc_num -1) {
			now_time = proc[endpoint + 1].ready_time;
		}
		finished++;
	}
	return ;
}
