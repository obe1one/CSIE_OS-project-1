typedef struct {
	char name[32];
	int ready_time;
	int exe_time;
} Process;

#define FIFO 0
#define RR 1
#define SJF 2
#define PSJF 3
