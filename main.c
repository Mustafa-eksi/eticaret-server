#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define _POSIX_C_SOURCE
#include <signal.h>
#define CMD_CAP 256

typedef enum DebugType {
    INFO,
    WARNING,
    ERROR
} DebugType;

void ms_debug(DebugType type, char* ms_name, char* message) {
    printf("%s (%s): %s\n", type == INFO ? "[INFO]" : type == WARNING ? "[WARNING]" : "[ERROR]", ms_name, message);
}

void kill_and_free(pid_t* processes, size_t size) {
    for(int i = 0; i < size; i++) {
        if(kill(processes[i], 9) != 0) {
            ms_debug(ERROR, "kill_and_free", "Couldn't send signal to child process!");
        }
    }
    free(processes);
}

void run_microservices(int argc, char* argv[]) {
    pid_t* child_pids = (pid_t*) malloc((argc-1)*sizeof(pid_t));
    int status = 0;
    for (size_t i = 1; i < argc; i++) { // Doesn't include program itself
        
        pid_t child_pid = fork();
        if(child_pid == -1) {
            ms_debug(ERROR, argv[i], "Couldn't create child process.");
            continue;
        }
        if(child_pid == 0) { // Child Process
            char buff[256] = {0};
            sprintf(buff, "cd %s; npm start", argv[i]);
            if(system(buff) != 0){
                //ms_debug(ERROR, argv[i], "System returned nonzero value");
                kill(getpid(), 9);
                continue;
            }
        } else { // Parent
            printf("Running microservice: %s, pid: %d\n", argv[i], (int)child_pid);
            child_pids[i] = child_pid;
            continue;
        }
    }
    while (wait(&status) > 0) {

        // REPL stuff
        //printf("-> ");
        /*char TmpBuff[CMD_CAP] = {0};
        read(0, TmpBuff, CMD_CAP); // Read from stdin
        if(strcmp(TmpBuff, "quit") == 0) { // if user written "quit"
            ms_debug(INFO, "server", "Killing all microservices.");
            kill_and_free(child_pids, argc-1);
        } else if (strcmp(TmpBuff, "childs") == 0) {
            for(int i = 1; i < argc; i++) {
                printf("Microservice name: %s - child id: %d\n", argv[i], child_pids[i-1]);
            }
        }*/


        //if() {
            ms_debug(INFO, "wait", "Waiting is over.");
            free(child_pids);
            return;
        //}
        
        
    }
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("ERROR: You must spesify at least one microservice to run\n");
        exit(-1);
    }
    run_microservices(argc, argv);
    return 0;
}
