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
