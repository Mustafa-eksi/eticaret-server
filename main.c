#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int run_in_subprocess(char* microservice) {
    pid_t child_pid = fork(), wpid;
    int status = 0;
    if(child_pid == -1) {
        printf("ERROR: Couldn't create child process.\n");
        exit(-1); // TODO: maybe it is just better to return -1
    }
    if(child_pid == 0) { // Child Process
        char buff[256] = {0};
        sprintf(buff, "cd %s; npm start", microservice);
        if(system(buff)){
        //if (execl("/usr/bin/npm", "run", "start", (char*) NULL) < 0) {
            printf("ERROR: Couldn't run npm\n");
            exit(-1); // TODO: maybe it is just better to return -1
        }
    } else { // Parent Process
        while ((wpid = wait(&status)) > 0);
        return child_pid;
    }
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("ERROR: You must spesify at least one microservice to run\n");
        exit(-1);
    }
    printf("this is first microservice: %s\n", argv[1]);
    int c_pid = run_in_subprocess(argv[1]);
    printf("child pid: %d\n", c_pid);
    
    return 0;
}