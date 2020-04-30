typedef struct {
	char name[32];
	int ready_time;
	int exe_time;
	int pid;
} Process;

#define ERR_EXIT(a) { perror(a) ; exit(1); }

#define TIMEQ 500

#define GETTIME 96
#define PRINK 333

#define LOW_PRIO -20
#define HIGHT_PRIO 20

#define FIFO 0
#define RR 1
#define SJF 2
#define PSJF 3

#define CPU0 0
#define CPU1 1

void unit_time();
void schd_FIFO(Process *proc, int proc_num);
void schd_RR(Process *proc, int proc_num);
void schd_SJF(Process *proc, int proc_num);
void schd_PSJF(Process *proc, int proc_num);

int proc_assign_cpu(int pid, int cpu);
int proc_get_ready(int pid);
int proc_exec(Process p);
int proc_exit_cpu(int pid);
