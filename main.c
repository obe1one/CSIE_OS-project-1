#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define _GNU_SOURCE
#include "config.h"

int compare(const void *x, const void *y)
{
	Process *a = (Process *)x;
	Process *b = (Process *)y;
	if(a->ready_time > b->ready_time) return 1;
	else return -1;
}

int main(int argc, char **argv)
{
	char schd_type[255];
	int proc_num;
	int policy = 0;
	Process *proc;
	
	scanf("%s", schd_type);
	scanf("%d", &proc_num);
	proc = (Process *)malloc(sizeof(Process) * proc_num);

	for(int i = 0; i < proc_num; i ++) {
		scanf("%s%d%d", proc[i].name, &proc[i].ready_time, &proc[i].exe_time);
		proc[i].pid = -1;
	}
	qsort(proc, proc_num, sizeof(Process), compare);
	for(int i = 0; i < proc_num; i ++) {
		printf("%s %d %d\n", proc[i].name, proc[i].ready_time, proc[i].exe_time);
	}

	if(strcmp(schd_type, "FIFO") == 0) {
		schd_FIFO(proc, proc_num);
	}
	else if(strcmp(schd_type, "RR") == 0) {
		schd_RR(proc, proc_num);
	}
	else if(strcmp(schd_type, "SJF") == 0) {
		schd_SJF(proc, proc_num);
	}
	else if(strcmp(schd_type, "PSJF") == 0) {
		schd_PSJF(proc, proc_num);
	}
	else {
		fprintf(stderr, "Wrong Type of scheduling\n");
		exit(1);
	}

}
