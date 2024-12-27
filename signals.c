#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"


void handle_sigint_ctrlC() {
    printf("p:%d\n",FGP_pid);
    if (FGP_pid != -1) {
        kill(FGP_pid, SIGINT);
    } 
}


void kill_all_processes() {
    Process* p = process_list;
    while (p != NULL) {
        if (kill(p->pid, SIGKILL) == -1) {
            // perror("Failed to kill process");
        } 
        else {
            // printf("Killed process with pid %d\n", p->pid);
        }
        p = p->next;
    }
}



//chk.................................................................

void handle_sigtstp_ctrlZ() {
    if (FGP_pid != -1) {
        kill(FGP_pid, SIGTSTP); 
        update_process_state(FGP_pid, 0); 
    }
}




void ping(char *argv[], int argc) {
    
    pid_t pid = atoi(argv[1]);
    int signalNo = atoi(argv[2]);

    signalNo = signalNo % 32;

    if (kill(pid, signalNo) == -1) {
        if (errno == ESRCH) {
            printf("No such process found\n");
        } 
        else {
            perror("kill");
        }
    } 
    else {
        printf("Sent signal %d to process with pid %d\n", signalNo, pid);
    }

}   