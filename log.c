#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"



void load_existing_commands() {
    char logFilePath[SIZ];
    snprintf(logFilePath, sizeof(logFilePath)+100, "%s/log_file.txt", homeDir);

    FILE *file = fopen(logFilePath, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    log_cmds_cnt = 0;
    while (fgets(log_store[log_cmds_cnt], SIZ, file) != NULL) {
        log_store[log_cmds_cnt][strcspn(log_store[log_cmds_cnt], "\n")] = '\0';
        log_cmds_cnt++;
        if (log_cmds_cnt >= MAX_LEN_LOG) {
            break;
        }
    }

    log_idx = log_cmds_cnt - 1;

    fclose(file);
}





void log_help1(char* inp){

    char logFilePath[SIZ];
    snprintf(logFilePath, sizeof(logFilePath)+100, "%s/log_file.txt", homeDir);

    if(strstr(inp, "log") != NULL) {
        return; // Don't store this inp
    }

    if (chk_purge == 1) {
        for (int i = 0; i < MAX_LEN_LOG; i++) {
            log_store[i][0] = '\0';
        }
        log_cmds_cnt = 0;
        log_idx = 0; 
        chk_purge = 0;
    }

    if (log_cmds_cnt > 0 && strcmp(last_cmd, inp) == 0) {
        return; // Don't store this inp
    }
    else {

        if (log_cmds_cnt >= MAX_LEN_LOG) {

            for (int i = 1; i < MAX_LEN_LOG; i++) {
                strcpy(log_store[i - 1], log_store[i]);
            }
            log_idx = MAX_LEN_LOG - 1;

        } else {
            log_idx = log_cmds_cnt;
            log_cmds_cnt++;
        }

        snprintf(log_store[log_idx], sizeof(log_store[log_idx]), "%s", inp);

        snprintf(last_cmd, sizeof(last_cmd), "%s", inp);

        FILE *file = fopen(logFilePath, "w");
        if (file == NULL) {
            perror("fopen");
            return;
        }

        for (int i = 0; i < log_cmds_cnt; i++) {
            fprintf(file, "%s\n", log_store[i]);
        }

        fclose(file);
       
    }

}



void log_print() {
    char logFilePath[SIZ];
    snprintf(logFilePath, sizeof(logFilePath)+100, "%s/log_file.txt", homeDir);

    int fd = open(logFilePath, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    char buff[SIZ];
    ssize_t bytesRead;
    
    while ((bytesRead = read(fd, buff, sizeof(buff))) > 0) {
        write(STDOUT_FILENO, buff, bytesRead);
    }
    
    close(fd);
}



void log_purge() {
    char logFilePath[SIZ];
    snprintf(logFilePath, sizeof(logFilePath)+100, "%s/log_file.txt", homeDir);

    int fd = open(logFilePath, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        perror("open");
        return;
    }
    close(fd);
}



void log_fun(char * argv[], int argc){

    if(strcmp(argv[0],"log")==0){
        if(argc==1){
            //log
            log_print();
        }
        else if(argc==2){
            //log purge
            if(strcmp(argv[1],"purge")==0){
                chk_purge=1;
                log_purge();
            }
            else{
                printf("Invalid command\n");
            }
        }
        else if(argc==3){
            //log execute <index>
            if(strcmp(argv[1],"execute")==0){

                int idxE=atoi(argv[2]);

                if(idxE<=0 || idxE>=16){
                    printf("Invalid index\n");
                }

                char inpE[SIZ];

                strcpy(inpE,log_store[log_cmds_cnt-idxE]);

                tokenize(inpE);


            }
            else{
                printf("Invalid command\n");
            }
        }
    }
}