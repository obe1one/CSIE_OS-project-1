typedef struct {
	char name[32];
	int ready_time;
	int exe_time;
	int is_execed;
} Process;

#define FIFO 0
#define RR 1
#define SJF 2
#define PSJF 3

void unit_time();
void schd_FIFO(Process *proc, int proc_num);
void schd_RR(Process *proc, int proc_num);
void schd_SJF(Process *proc, int proc_num);
void schd_PSJF(Process *proc, int proc_num);
