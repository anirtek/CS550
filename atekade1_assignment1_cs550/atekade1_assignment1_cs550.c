#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    int h, c; /*Input Variables 'h' is height & 'c' is child*/
    int wpid, status;
    printf("\nInput Height : ");
    scanf("%d", &h); 
    printf("\nInput Number of Child processes : ");
    scanf("%d", &c);

    int pid;

    printf("(%d) : process starting\n", getpid());
    printf("(%d) : parent's id = (%d)\n", getpid(), getppid());
    printf("(%d) : height in the tree = %d\n", getpid(), h);
    printf("(%d) : Creating %d child at height %d\n", getpid(), c, h-1);

    if(h > 1) {
        for(int i = 0; i < c; i++) {
            pid = fork(); //Creating the child

            if(pid > 0) { /*parent block*/
                //parent
                continue;
            } 
            else {
                if(pid == 0) {
                    printf("(%d) : process starting\n", getpid());
                    printf("(%d) : parent's id = (%d)\n", getpid(), getppid());
                    printf("(%d) : height in the tree = %d\n", getpid(), h);
                    printf("(%d) : Creating %d child at height %d\n", getpid(), c, h-1);

                    h--;
                    execl("./atekade_assignment1_cs550", "h", "c", (char *) 0);
                    break;
                } else {
                    printf("fork error\n");
                    exit(1);
                }
            }   

        }
    }

    while((wpid == wait(&status)) > 0);
    return 0;
}
