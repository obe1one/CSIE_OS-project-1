#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage void sys_printk(char *demsg)
{
	printk("%s\n", msg);
	return ;
}
