#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"


void proclore_help(pid_t pid){
    char path1[SIZ];
    snprintf(path1, sizeof(path1), "/proc/%d/status", pid);

    FILE *file = fopen(path1, "r");
    if (file==NULL) {
        perror("fopen");
        return;
    }

    printf("pid : %d\n",pid);


    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "State:", 6) == 0) {
            char temp[3];
            sscanf(line, "State:\t%s", temp);
            if (PID_SHELL==pid) {
                strcat(temp, "+");
            }
            printf("process Status : %s\n", temp);
        } 
        else if (strncmp(line, "Pid:", 4) == 0) {
            continue;
        } 
        else if (strncmp(line, "PPid:", 5) == 0) {
            printf("Process Group: %s", line + 6); 
        } 
        else if (strncmp(line, "VmSize:", 7) == 0) {
            printf("Virtual memory: %s", line + 8); 
        }
    }

    fclose(file);

    char path2[SIZ];
    snprintf(path2, sizeof(path2), "/proc/%d/exe", pid);

    char exePath[SIZ];

    ssize_t l = readlink(path2, exePath, sizeof(exePath) - 1);
    if (l != -1) {
        exePath[l] = '\0';
        printf("executable Path: %s\n", exePath);
    } else {
        perror("readlink");
    }

}

void proclore_ntg(){
    proclore_help(PID_SHELL);
}

void proclore_pid(pid_t pid){
    proclore_help(pid);
}

void proclore(char *argv[], int argc) {

    if(argc==1 && strcmp(argv[0],"proclore")==0){
        proclore_ntg();
    }
    else if(argc==2){
        int pidE=atoi(argv[1]);
        proclore_pid(pidE);
    }
}