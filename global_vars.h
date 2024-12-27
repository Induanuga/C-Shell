#ifndef GV_H
#define GV_H
#include "headers.h"

extern char prevDir[SIZ];
extern char currDir[SIZ];
extern char homeDir[SIZ];

extern char prompt_path[SIZ];

extern char log_store[MAX_LEN_LOG][SIZ];
extern int log_idx;
extern int log_cmds_cnt;
extern char last_cmd[SIZ];
extern int chk_purge;

extern pid_t PID_SHELL;

extern int cmdT_gt2_flag;
extern char cmd_gt2_add[SIZ];

extern int saved_stdin;
extern int saved_stdout;


typedef struct Process {
    int pid;
    char command[256];
    int isRunning;
    struct Process* next;
} Process;
extern Process* process_list;


extern pid_t most_recently_created_pid;

extern pid_t FGP_pid;

#endif