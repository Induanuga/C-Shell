#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"



void fg(char *argv[], int argc) {
    if (argc < 2) {
        printf("Usage: fg <pid>\n");
        return;
    }

    pid_t pid = atoi(argv[1]);
    Process* p = process_list;
    Process* givenProcess = NULL;

    while (p != NULL) {
        if (p->pid == pid) {
            givenProcess = p;
            break;
        }
        p = p->next;
    }

    if (givenProcess == NULL) {
        printf("No such process found\n");
        return;
    }

    if (givenProcess->isRunning == 0) {
        if (kill(pid, SIGCONT) == -1) {
            perror("SIGCONT");
            return;
        }
        givenProcess->isRunning = 1;
    }

    FGP_pid = pid;
    int status;
    if (waitpid(pid, &status, WUNTRACED) == -1) {
        perror("Error waiting for process");
        return;
    }

    if (WIFSTOPPED(status)) {
        givenProcess->isRunning = 0;
    } 
    else if (WIFEXITED(status) || WIFSIGNALED(status)) {
        remove_process(pid);
    }

    FGP_pid = -1;
}




void bg(char *argv[], int argc) {
    if (argc < 2) {
        printf("Usage: bg <pid>\n");
        return;
    }

    pid_t pid = atoi(argv[1]);
    Process* p = process_list;
    Process* givenProcess = NULL;

    while (p != NULL) {
        if (p->pid == pid) {
            givenProcess = p;
            break;
        }
        p = p->next;
    }

    if (givenProcess == NULL) {
        printf("No such process found\n");
        return;
    }

    if (givenProcess->isRunning == 0) {
        if (kill(pid, SIGCONT) == -1) {
            perror("SIGCONT");
            return;
        }
        givenProcess->isRunning = 1;
        printf("Process [%d] resumed in the background\n", pid);
    } 
    else {
        printf("Process [%d] is already running\n", pid);
    }
}
