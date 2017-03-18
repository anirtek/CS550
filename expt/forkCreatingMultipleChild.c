#include<stdio.h>
#include<unistd.h>

void main() {

    int pid;
    
    for(int i=1; i<=2; i++){
    
        pid = fork();
        
        if(pid == 0) printf("This is child %d process with pid = %d\n", i, pid);
        else if(pid > 0) printf("This is parent %d process with pid = %d\n", i, pid);
        else printf("Fork returned error while creating child\n");
    }
}
