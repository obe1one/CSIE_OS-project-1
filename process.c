#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "config.h"
#include <sched.h>
#include <sys/resource.h>

void unit_time()
{
	volatile unsigned long i;
	for(i = 0; i < 1000000UL; i++);
	return ;
}

int proc_assign_cpu(int pid, int cpu)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(cpu, &mask);

	if(sched_setaffinity(pid, sizeof(mask), &mask) < 0) 
		ERR_EXIT("sched_setaffinity\n");
	return 0;
}// determine the set of CPUs on which it is eligible to run

int proc_get_ready(int pid)
{
	struct sched_param param;
	param.sched_priority = 0;

	int en = sched_setscheduler(pid, SCHED_OTHER, &param);

	if(en < 0) ERR_EXIT("proc get ready other\n");
	
	return en;
}

int proc_exec(Process p)
{
	int pid;
	if((pid = fork()) == 0) {
		for(int i = 0; i < p.exe_time; i++) {
			unit_time();
		}
		exit(1);
	}
	proc_assign_cpu(pid, CPU1);
	return pid;
}

int proc_exit_cpu(int pid)
{
	struct sched_param param;
	param.sched_priority = 0;

	int en = sched_setscheduler(pid, SCHED_IDLE, &param);

	if(en < 0) ERR_EXIT("sched_setscheduler, idle\n");
	return en;
}

