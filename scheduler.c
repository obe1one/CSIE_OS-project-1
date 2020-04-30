#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "config.h"
#include <sched.h>

void schd_FIFO(Process *proc, int proc_num)
{
	int now_proc = -1;
	int now_time = 0;
	int finished = 0;
	int ready_proc_num = 0;

	proc_assign_cpu(getpid(), CPU0);
	proc_get_ready(getpid());

	while(1) {
		//set the ready processes
		for(int i = ready_proc_num; i < proc_num; i++) {
			if(proc[i].ready_time == now_time) {
				proc[i].pid = proc_exec(proc[i]);
				proc_exit_cpu(proc[i].pid);
				ready_proc_num++;

			}
			else break;
		}

		if(now_proc == -1) {
			if(proc[finished].pid != -1) {
				now_proc = finished;
				proc_get_ready(proc[now_proc].pid);
			}
		}

		if(now_proc != -1) {
			proc[now_proc].exe_time--;
		}

			
		if(now_proc != -1 && proc[now_proc].exe_time == 0) {
			int statloc;
			waitpid(proc[now_proc].pid, &statloc, 0);
			printf("%s %d\n", proc[now_proc].name, proc[now_proc].pid);
			now_proc = -1;
			finished++;
			if(finished == proc_num) break;
		}
		unit_time();
		now_time++;
	}
	return ;
}

int select_SJF(Process *proc, int ready_proc_num)
{
	int index = -1;
	int min_time = 100000000;
	for(int i = 0; i < ready_proc_num; i ++) {
		if(proc[i].exe_time < min_time && proc[i].exe_time != 0) {
			index = i;
			min_time = proc[i].exe_time;
		}
	}
	return index;
}


void schd_SJF(Process *proc, int proc_num)
{
	int now_proc = -1;
	int now_time = 0;
	int finished = 0;
	int ready_proc_num = 0;

	proc_assign_cpu(getpid(), CPU0);
	proc_get_ready(getpid());

	while(1) {
		for(int i = ready_proc_num; i < proc_num; i++) {
			if(proc[i].ready_time == now_time) {
				proc[i].pid = proc_exec(proc[i]);
				proc_exit_cpu(proc[i].pid);
				ready_proc_num++;
			}
			else break;
		}

		if(now_proc == -1) {
			int index = select_SJF(proc, ready_proc_num);
			if(index != -1) {
				now_proc = index;
				proc_get_ready(proc[now_proc].pid);
			}
		}

		if(now_proc != -1) {
			proc[now_proc].exe_time--;
		}

		if(now_proc != -1 && proc[now_proc].exe_time == 0) {
			int statloc;
			waitpid(proc[now_proc].pid, &statloc, 0);
			printf("%s %d\n", proc[now_proc].name, proc[now_proc].pid);
			now_proc = -1;
			finished++;
			if(finished == proc_num) break;
		}
		unit_time();
		now_time++;
	}
	return ;

}

int select_PSJF(Process *proc, int ready_proc_num)
{
	int index = -1;
	int min_time = 100000000;
	for(int i = 0; i < ready_proc_num; i++) {
		if(proc[i].exe_time < min_time && proc[i].exe_time != 0) {
			index = i;
			min_time = proc[i].exe_time;
		}
	}
	return index;
}

void schd_PSJF(Process *proc, int proc_num)
{
	int now_proc = -1;
	int now_time = 0;
	int finished = 0;
	int ready_proc_num = 0;

	proc_assign_cpu(getpid(), CPU0);
	proc_get_ready(getpid());

	while(1) {
		for(int i = ready_proc_num; i < proc_num; i++) {
			if(proc[i].ready_time == now_time) {
				proc[i].pid = proc_exec(proc[i]);
				proc_exit_cpu(proc[i].pid);
				ready_proc_num++;
			}
			else break;
		}

		int index = select_PSJF(proc, ready_proc_num);

		if(index != -1) {
			if(now_proc != index) {
				now_proc = index;
				proc_get_ready(proc[now_proc].pid);
			}
		}
		
		if(now_proc != -1) {
			proc[now_proc].exe_time--;
		}

		if(now_proc != -1 && proc[now_proc].exe_time == 0) {
			int statloc;
			waitpid(proc[now_proc].pid, &statloc, 0);
			printf("%s %d\n", proc[now_proc].name, proc[now_proc].pid);
			now_proc = -1;
			finished++;
			if(finished == proc_num) break;
		}
		unit_time();
		now_time++;
	}
	return ;

}

int select_RR(Process *proc, int ready_proc_num, int prev_proc)
{
	if(ready_proc_num == 0) return -1;
	for(int i = 1; i < ready_proc_num; i++) {
		if(proc[(i + prev_proc) % ready_proc_num].exe_time != 0)
			return (i + prev_proc) % ready_proc_num;
	}
	if(prev_proc == -1) return 0;
	if(proc[prev_proc].exe_time != 0) return prev_proc;
	else return -1;
}

void schd_RR(Process *proc, int proc_num)
{
	int now_proc = -1;
	int prev_proc = -1;
	int now_time = 0;
	int finished = 0;
	int ready_proc_num = 0;
	int time_qun = 0;

	proc_assign_cpu(getpid(), CPU0);
	proc_get_ready(getpid());

	while(1) {
		for(int i = ready_proc_num; i < proc_num; i++) {
			if(proc[i].ready_time == now_time) {
				proc[i].pid = proc_exec(proc[i]);
				proc_exit_cpu(proc[i].pid);
				ready_proc_num++;
			}
			else break;
		}

		if(time_qun >= TIMEQ) {
			time_qun = 0;
			prev_proc = now_proc;
			now_proc = -1;
		}

		if(now_proc == -1) {
			int index = select_RR(proc, ready_proc_num, prev_proc);
			if(index != -1) {
				now_proc = index;
				time_qun = 0;
				proc_get_ready(proc[now_proc].pid);
				printf("context switch, now_proc = %d, now_time = %d\n", now_proc, now_time);
			}
		}

		if(now_proc != -1) {
			proc[now_proc].exe_time--;
			time_qun++;
		}

		if(now_proc != -1 && proc[now_proc].exe_time == 0) {
			int statloc;
			waitpid(proc[now_proc].pid, &statloc, 0);
			printf("%s %d\n", proc[now_proc].name, proc[now_proc].pid);
			prev_proc = now_proc;
			now_proc = -1;
			time_qun = 0;
			finished++;
			if(finished == proc_num) break;
		}
		unit_time();
		now_time++;
	}
	return ;
}




