#include "global_vars.h"
#include "headers.h"


char prevDir[SIZ];
char currDir[SIZ];
char homeDir[SIZ];

char prompt_path[SIZ];

char log_store[MAX_LEN_LOG][SIZ];
int log_idx=0;
int log_cmds_cnt=0;
char last_cmd[SIZ]="";
int chk_purge=0;

pid_t PID_SHELL;

int cmdT_gt2_flag=0;
char cmd_gt2_add[SIZ]="";

int saved_stdin;
int saved_stdout;


Process* process_list = NULL;


pid_t most_recently_created_pid=-1;


pid_t FGP_pid=-1;