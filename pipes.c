#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"


void pipe_wth_redirctn(char* pipe_cmds[], int argc_Pipes) {
    int prev_fd = -1; 
    for (int i = 0; i < argc_Pipes; i++) {
        int pipe_fd[2];
        if (i != argc_Pipes - 1) {
            if (pipe(pipe_fd) == -1) {
                perror("pipe");
                return;
            }
        }
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return;
        }
        else if (pid == 0) { 
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO); 
                close(prev_fd);
            }
            if (i != argc_Pipes - 1) {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);  
                close(pipe_fd[1]);
            }
            redirctn_fn(pipe_cmds[i]);
            exit(0);
        }
        else {  


            add_process(pid, pipe_cmds[i], 1);
            int status;
            waitpid(pid, &status, WUNTRACED);


            if (prev_fd != -1) {
                close(prev_fd);
            }
            if (i != argc_Pipes - 1) {
                close(pipe_fd[1]);
                prev_fd = pipe_fd[0];
            }
        }
    }

    for (int i = 0; i < argc_Pipes; i++) {
        wait(NULL);
    }
}



char* trimSpaces(char* str) {
    while (isspace(*str)) str++;
    if (*str == 0) return str;  
    char* end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--; 
    *(end + 1) = 0; 
    return str;
}


int blank_str(char* str) {
    while (*str) {
        if (!isspace(*str)) 
        return 0; 
        str++;
    }
    return 1; 
}


int chk_invalid(char* cmd) {
    cmd = trimSpaces(cmd);
    if (*cmd == '|' || *(cmd + strlen(cmd) - 1) == '|') {
        return 1;
    }
    int prev_char_pipe = 0;
    int i = 0;
    while (cmd[i] != '\0') {
        if (cmd[i] == '|') {
            if (prev_char_pipe) {
                return 1; 
            }
            prev_char_pipe = 1;
        } 
        else {
            prev_char_pipe = 0;
        }
        i++;
    }
    return 0;
}


void handle_pipes(char* cmd) {
    char* pipe_cmds[1000];
    int argc_Pipes = 0;
    char* pipe_tok;
    char* pipe_ptr;
    if (chk_invalid(cmd)) {
        fprintf(stderr, "Invalid use of pipe\n");
        return;
    }
    pipe_tok = strtok_r(cmd, "|", &pipe_ptr);
    while (pipe_tok != NULL) {
        pipe_tok = trimSpaces(pipe_tok); 
        if (blank_str(pipe_tok)) {
            fprintf(stderr, "Invalid use of pipe\n");
            return; 
        }
        pipe_cmds[argc_Pipes] = malloc((strlen(pipe_tok) + 1) * sizeof(char));
        strcpy(pipe_cmds[argc_Pipes++], pipe_tok);
        pipe_tok = strtok_r(NULL, "|", &pipe_ptr);
    }
    pipe_cmds[argc_Pipes] = NULL;
    pipe_wth_redirctn(pipe_cmds, argc_Pipes);
    for (int i = 0; i < argc_Pipes; i++) {
        free(pipe_cmds[i]);
    }
}
