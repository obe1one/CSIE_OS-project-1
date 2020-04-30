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
		}

		if(now_proc == -1) {
			if(proc[finished].pid != -1) {
				now_proc = finished;
				proc_get_ready(proc[now_proc].pid);
			}
		}
		else {
			proc[now_proc].exe_time--;
		}

			
		if(proc[now_proc].exe_time == 0) {
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



