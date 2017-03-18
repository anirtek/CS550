/***************************************************************************
 * This is the user level and user space limited program that calls to the
 * kernel module called "process_list". 
 *
 * In return, process_list returns the PID, PPID, CPU and STATE of the processes
 * one by one from the kernel space to the user space.
 *
 * This is done by making a simple read() call onto the kernel module running at
 * present.
 * *************************************************************************
 */

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
struct process
{
	int pid;
	int ppid;
	int cpu;
	int state;
};

int main()
{
    /**
     * Enumed status of the processes
     */
	enum {
	TASK_RUNNING = 0,
	TASK_INTERRUPTIBLE = 1,
	TASK_UNINTERRUPTIBLE = 2,
	__TASK_STOPPED = 4,
	__TASK_TRACED = 8,
	EXIT_DEAD = 16,
	EXIT_ZOMBIE = 32,
	EXIT_TRACE = EXIT_ZOMBIE | EXIT_DEAD,
	TASK_DEAD = 64,
	TASK_WAKEKILL = 128,
	TASK_WAKING = 256,
	TASK_PARKED = 512,
	TASK_NOLOAD = 1024,
	TASK_NEW = 2048,
	TASK_STATE_MAX = 4096,
	TASK_KILLABLE = TASK_WAKEKILL | TASK_UNINTERRUPTIBLE, 
	TASK_STOPPED = TASK_WAKEKILL | __TASK_STOPPED,
	TASK_TRACED = TASK_WAKEKILL | __TASK_TRACED,
	TASK_IDLE = TASK_UNINTERRUPTIBLE | TASK_NOLOAD,
	TASK_NORMAL = TASK_INTERRUPTIBLE | TASK_UNINTERRUPTIBLE, 
	TASK_ALL = TASK_NORMAL | __TASK_STOPPED | __TASK_TRACED,
	TASK_REPORT = TASK_RUNNING | TASK_INTERRUPTIBLE | TASK_UNINTERRUPTIBLE | __TASK_STOPPED | __TASK_TRACED | EXIT_ZOMBIE | EXIT_DEAD
	};

	int fd=0,ret=0;
	int len = 512;
	struct process p;

    /**
     * Making read() call on the live kernel module inserted recently.
     */
	fd=open("/dev/process_list",O_RDONLY);
	if(fd == -1)
		perror("File opening error!");


	while(ret != -1)
	{
		ret=read(fd, &p, sizeof(struct process));
		if(ret == 0)
		{
			printf("PID =%7d\t PPID =%7d\t CPU =%3d\t ", p.pid, p.ppid, p.cpu);
			int state = p.state;
			char arr[100];
			switch(state)
			{
				case TASK_RUNNING:
					strcpy(arr, "TASK_RUNNING");
					printf("STATE = %s", arr);
					break;
				case TASK_INTERRUPTIBLE:
					strcpy(arr, "TASK_INTERRUPTABLE");
					printf("STATE = %s", arr);
					break;
				case TASK_UNINTERRUPTIBLE:
					strcpy(arr, "TASK_UNINTERRUPTIBLE");
					printf("STATE = %s", arr);
					break;
				case __TASK_STOPPED:
					strcpy(arr, "__TASK_STOPPED");
					printf("STATE = %s", arr);
					break;
				case __TASK_TRACED:
					strcpy(arr, "__TASK_TRACED");
					printf("STATE = %s", arr);
					break;
				case EXIT_DEAD:
					strcpy(arr, "EXIT_DEAD");
					printf("STATE = %s", arr);
					break;
				case EXIT_ZOMBIE:
					strcpy(arr, "EXIT_ZOMBIE");
					printf("STATE = %s", arr);
					break;
				case EXIT_TRACE:
					strcpy(arr, "EXIT_TRACE");
					printf("STATE = %s", arr);
					break;
				case TASK_DEAD:
					strcpy(arr, "TASK_DEAD");
					printf("STATE = %s", arr);
					break;
				case TASK_WAKEKILL:
					strcpy(arr, "TASK_WAKEKILL");
					printf("STATE = %s", arr);
					break;
				case TASK_WAKING:
					strcpy(arr, "TASK_WAKING");
					printf("STATE = %s", arr);
					break;
				case TASK_PARKED:
					strcpy(arr, "TASK_PARKED");
					printf("STATE = %s", arr);
					break;
				case TASK_NOLOAD:
					strcpy(arr, "TASK_NOLOAD");
					printf("STATE = %s", arr);
					break;
				case TASK_NEW:
					strcpy(arr, "TASK_NEW");
					printf("STATE = %s", arr);
					break;
				case TASK_STATE_MAX:
					strcpy(arr, "TASK_STATE_MAX");
					printf("STATE = %s", arr);
					break;
				case TASK_KILLABLE:
					strcpy(arr, "TASK_KILLABLE");
					printf("STATE = %s", arr);
					break;
				case TASK_STOPPED:
					strcpy(arr, "TASK_STOPPED");
					printf("STATE = %s", arr);
					break;
				case TASK_TRACED:
					strcpy(arr, "TASK_TRACED");
					printf("STATE = %s", arr);
					break;
				case TASK_IDLE:
					strcpy(arr, "TASK_IDLE");
					printf("STATE = %s", arr);
					break;
				case TASK_NORMAL:
					strcpy(arr, "TASK_NORMAL");
					printf("STATE = %s", arr);
					break;
				case TASK_ALL:
					strcpy(arr, "TASK_ALL");
					printf("STATE = %s", arr);
					break;
				case TASK_REPORT:
					strcpy(arr, "TASK_REPORT");
					printf("STATE = %s", arr);
					break;

			}
			printf("\n");
		} 
		else break;
	}
    
    /**
     * Close the file desciptor.
     */
	close(fd);
}
