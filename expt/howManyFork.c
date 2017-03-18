#include<unistd.h>
#include<stdio.h>

void main()
{
       int count = 0; 
       int pid;
    for(int i = 0; i < 1; i++) {
        pid = fork();
        if(pid == 0) printf("forked! with %d child\n", counter++);
        else if(pid > 0) printf("forked! with %d parent\n", counter++);
    }
   
}
