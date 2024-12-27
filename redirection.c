#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"


void redirctn_fn(char* cmd) {
    char* IP_file = NULL;
    char* OP_file = NULL;
    int appendOrNot = 0; 
    char* args[256];
    int argc = 0;
    char* tok;
    char* ptr;
    
    tok = strtok_r(cmd, " \t\n", &ptr);
    
    while (tok != NULL) {
        if (strcmp(tok, "<") == 0) {
            tok = strtok_r(NULL, " \t\n", &ptr);
            if (tok != NULL) {
                IP_file = tok;
            }
            else {
                fprintf(stderr, "Error: No input file specified after '<'\n");
                return;
            }
        } 
        else if (strcmp(tok, ">") == 0) {
            tok = strtok_r(NULL, " \t\n", &ptr);
            if (tok != NULL) {
                OP_file = tok;
                appendOrNot = 0;
            } 
            else {
                fprintf(stderr, "Error: No output file specified after '>'\n");
                return;
            }
        } 
        else if (strcmp(tok, ">>") == 0) {
            tok = strtok_r(NULL, " \t\n", &ptr);
            if (tok != NULL) {
                OP_file = tok;
                appendOrNot = 1;
            } 
            else {
                fprintf(stderr, "Error: No output file specified after '>>'\n");
                return;
            }
        } 
        else {

            //check..................................................................................

            // if (tok[0] == '\'' || tok[0] == '\"') {
            // } 
            // else {
                args[argc++] = tok;
            // }
        }

        tok = strtok_r(NULL, " \t\n", &ptr);
    }

    args[argc] = NULL;

    if (args[0] == NULL) return;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    } 
    else if (pid == 0) {
        if (IP_file != NULL) {
            int IP_fd = open(IP_file, O_RDONLY);
            if (IP_fd < 0) {
                if (errno == ENOENT) {
                    fprintf(stderr, "Error: No such input file found!\n");
                } 
                else {
                    perror("open");
                }
                return;
            }
            dup2(IP_fd, STDIN_FILENO);
            close(IP_fd);
        }

        if (OP_file != NULL) {
            int OP_fd;
            if (appendOrNot == 1) {
                OP_fd = open(OP_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            } 
            else {
                OP_fd = open(OP_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            if (OP_fd < 0) {
                perror("open");
                return;
            }
            dup2(OP_fd, STDOUT_FILENO);
            close(OP_fd);
        }

        fun_FG(args,argc);
    } 
    else {
        add_process(pid, cmd, 1);
        int status;
        waitpid(pid, &status, WUNTRACED);
        // update_process_state(pid, 0);
    }
}



void redirctn_handling_without_pipes_fg_and_bg(char * cmd){
    char* RD_BGPs[450];
    int NoOf_RD_BGPs=0;
    char* RD_FGPs[450];
    int NoOf_RD_FGPs=0;

    int len2=strlen(cmd);
    
    for(int i=0;i<len2;i++){
        
        char* strUse1 = (char*)malloc((len2 + 1) * sizeof(char));
        if (strUse1 == NULL) {
            perror("malloc");
            return;
        }
        int indUse1=0;

        while(i<len2){
            if(cmd[i]=='&'){
                strUse1[indUse1++]=cmd[i++];
                break;    
            }
            strUse1[indUse1++]=cmd[i++];
        }
        strUse1[indUse1]='\0';

        if(strUse1[indUse1-1]=='&'){
            strUse1[indUse1-1]='\0';
            RD_BGPs[NoOf_RD_BGPs] = (char *)malloc((strlen(strUse1) + 1) * sizeof(char));
            if (RD_BGPs[NoOf_RD_BGPs] == NULL) {
                perror("malloc");
                return;
            }
            strcpy(RD_BGPs[NoOf_RD_BGPs++],strUse1);
        }
        else{
            RD_FGPs[NoOf_RD_FGPs] = (char *)malloc((strlen(strUse1) + 1) * sizeof(char));
            if (RD_FGPs[NoOf_RD_FGPs] == NULL) {
                perror("malloc");
                return;
            }
            strcpy(RD_FGPs[NoOf_RD_FGPs++],strUse1);
        }
        free(strUse1);
    }
    

    for(int j=0;j<NoOf_RD_BGPs;j++){

        // handle_BGP();

        char store1[SIZ];
        strcpy(store1,RD_BGPs[j]);

        char* temp2=RD_BGPs[j];
        char* subtok2;
        char* ptr3;
        char* argv_RD_BGP[100];
        int argc_RD_BGP=0;

        subtok2=strtok_r(temp2," \t",&ptr3);
        while(subtok2!=NULL){
            argv_RD_BGP[argc_RD_BGP]=(char*)malloc((strlen(subtok2)+1)*sizeof(char));
            strcpy(argv_RD_BGP[argc_RD_BGP++],subtok2);
            subtok2=strtok_r(NULL," \t",&ptr3);
        }
        argv_RD_BGP[argc_RD_BGP] = NULL;


        if(strchr(store1, '<') != NULL || 
        strchr(store1, '>') != NULL || 
        strstr(store1, ">>") != NULL) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                return;
            } 
            else if (pid == 0) {
                redirctn_fn(store1);
                exit(0);
            }
            else {
                add_process(pid, store1, 1);
                printf("%d\n", pid);
            }
        }
        else{
            fun_BG(argv_RD_BGP,argc_RD_BGP);
        }

        for (int i = 0; i < argc_RD_BGP; i++) {
            free(argv_RD_BGP[i]);
        }
    }

    for(int j=0;j<NoOf_RD_FGPs;j++){

        // handle_BGP();

        char store1[SIZ];
        strcpy(store1,RD_FGPs[j]);

        char* temp1=RD_FGPs[j];
        char* subtok1;
        char* ptr2;
        char* argv_RD_FGP[256];
        int argc_RD_FGP=0;

        subtok1=strtok_r(temp1," \t",&ptr2);
        while(subtok1!=NULL){
            argv_RD_FGP[argc_RD_FGP]=(char*)malloc((strlen(subtok1)+1)*sizeof(char));
            strcpy(argv_RD_FGP[argc_RD_FGP++],subtok1);
            subtok1=strtok_r(NULL," \t",&ptr2);
        }
        argv_RD_FGP[argc_RD_FGP] = NULL;

        if(strchr(store1, '<') != NULL || 
        strchr(store1, '>') != NULL || 
        strstr(store1, ">>") != NULL) {
            redirctn_fn(store1);
        }
        else{
            fun_FG(argv_RD_FGP,argc_RD_FGP);
        }

        for (int i = 0; i < argc_RD_FGP; i++) {
            free(argv_RD_FGP[i]);
        }
    }


    for (int i = 0; i < NoOf_RD_BGPs; i++) {
        free(RD_BGPs[i]);
    }
    for (int i = 0; i < NoOf_RD_FGPs; i++) {
        free(RD_FGPs[i]);
    }

}

