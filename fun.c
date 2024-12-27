#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"


void add_process(int pid, char* cmd, int isRunning) {
    Process* p = (Process*)malloc(sizeof(Process));
    p->pid = pid;
    strncpy(p->command, cmd, sizeof(p->command));
    p->isRunning = isRunning;
    p->next = process_list;
    process_list = p;

    most_recently_created_pid=pid;
}


void update_process_state(int pid, int isRunning) {
    Process* p = process_list;
    while (p) {
        if (p->pid == pid) {
            p->isRunning = isRunning;
            return;
        }
        p = p->next;
    }
}


void remove_process(int pid) {
    Process* p = process_list;
    Process* prev = NULL;
    while (p) {
        if (p->pid == pid) {
            if (prev) {
                prev->next = p->next;
            } else {
                process_list = p->next;
            }
            free(p);
            return;
        }
        prev = p;
        p = p->next;
    }
}




void fun_FG(char * argv_FGP[], int argc_FGP){

    if(strcmp(argv_FGP[0],"hop")==0){
        hop(argv_FGP,argc_FGP);
    }
    else if(strcmp(argv_FGP[0],"reveal")==0){
        reveal(argv_FGP,argc_FGP);
    }
    else if(strcmp(argv_FGP[0],"log")==0){
        log_fun(argv_FGP,argc_FGP);
    }
    else if(strcmp(argv_FGP[0],"proclore")==0){
        proclore(argv_FGP,argc_FGP);
    }
    else if(strcmp(argv_FGP[0],"seek")==0){
        seek(argv_FGP,argc_FGP);
    }
    else if(strcmp(argv_FGP[0],"activities")==0){
        activities();
    }
    else if(strcmp(argv_FGP[0],"neonate")==0){
        neonate(argv_FGP,argc_FGP);
    }
    else if(strcmp(argv_FGP[0],"ping")==0){
        ping(argv_FGP,argc_FGP);
    }
    else if(strcmp(argv_FGP[0],"fg")==0){
        fg(argv_FGP,argc_FGP);
    }
    else if(strcmp(argv_FGP[0],"bg")==0){
        bg(argv_FGP,argc_FGP);
    }
    else if(strcmp(argv_FGP[0],"iMan")==0){
        iMan(argv_FGP,argc_FGP);
    }
    else{
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            return;
        } 
        else if (pid == 0) {
            execvp(argv_FGP[0], argv_FGP);
            fprintf(stderr, "\x1b[31mERROR: '%s' is not a valid command\n\x1b[0m", argv_FGP[0]);
            return;
        } 
        else {

            struct timeval start_time, end_time;
            gettimeofday(&start_time, NULL);

            add_process(pid, argv_FGP[0], 1);

            FGP_pid = pid;

            int status;
            
            waitpid(pid, &status, WUNTRACED);

            if (WIFSTOPPED(status)) {
                printf("Foreground process with PID %d has been stopped and moved to background.\n", pid);
                update_process_state(pid, 0);
            } 
            else if (WIFEXITED(status)) {
                FGP_pid = -1;
            } 
            else if (WIFSIGNALED(status)) {
                FGP_pid = -1;
            }


            gettimeofday(&end_time, NULL);

            double time_taken = (end_time.tv_sec - start_time.tv_sec) + ((end_time.tv_usec - start_time.tv_usec) / 1000000.0);

            if (time_taken > 3.0) {

                cmdT_gt2_flag=1;

                char frmt[SIZ];
                snprintf(frmt, sizeof(frmt), "%s : %ds", argv_FGP[0], (int)time_taken);

                if (strlen(prompt_path) > 0) {
                    strcat(cmd_gt2_add, " ");
                }
                strcat(cmd_gt2_add, frmt);

            }
            // update_process_state(pid, 0);
        }
    }
}



void fun_BG(char * argv_BGP[], int argc_BGP){
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    else if(pid == 0) {
        setpgid(0,0);
        execvp(argv_BGP[0], argv_BGP);
        perror("execvp");
        return;
    } 
    else if (pid > 0) {
        add_process(pid, argv_BGP[0], 1);
        printf("%d\n", pid);
    } 
}



// void handle_BGP() {
//     int status;
//     pid_t pid;
//     while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
//         if (WIFEXITED(status)) {
//             printf("Process %d exited normally\n", pid);
//             remove_process(pid);
//         } 
//         else if (WIFSIGNALED(status)) {
//             printf("Process %d terminated by signal %d\n", pid, WTERMSIG(status));
//             remove_process(pid);  
//         } 
//         else if (WIFSTOPPED(status)) {
//             printf("Process %d stopped by signal %d\n", pid, WSTOPSIG(status));
//             update_process_state(pid, 0); 
//         } 
//         else if (WIFCONTINUED(status)) {
//             printf("Process %d continued\n", pid);
//             update_process_state(pid, 1); 
//         }
//     }
//     if (pid == -1 && errno != ECHILD) {
//         perror("waitpid");
//     }
// }

