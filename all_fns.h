#ifndef ALLFNS_H
#define ALLFNS_H


//tokenize.c
void tokenize(char inp[SIZ]);



//hop.c
int readPermission(char *fp);
int validDir(char * fp);
void handling_prompt();
void hop_ntg_or_tilde();
void hop_hyphen();
void hop_specific(char *path);
void hop(char * argv[], int argc);



//fun.c
void add_process(int pid, char* cmd, int isRunning);
void update_process_state(int pid, int isRunning);
void remove_process(int pid);
void fun_FG(char * argv_FGP[], int argc_FGP);
void fun_BG(char * argv_BGP[], int argc_BGP);




//reveal.c
int compare(const void * a, const void * b);
void chkPermissions(mode_t mode, char *str);
void showFandD(char *path, int a_flag, int l_flag);
void reveal(char * argv[], int argc);




//log.c
void load_existing_commands();
void log_help1(char* cmd);
void log_print();
void log_purge();
void log_fun(char * argv[], int argc);




//proclore.c
void proclore_help(pid_t pid);
void proclore_ntg();
void proclore_pid(pid_t pid);
void proclore(char *argv[], int argc);



//seek.c
void printFileCont(char *path);
void changeDir(char* path);
void searchDir(char* search, char* basePath, char* currPath, int f_flag, int d_flag, int e_flag, int * No_ofFD_fnd, char*  one_match_fp);
void seek(char *argv[], int argc);



//activities.c
int compare_processes(const void* a, const void* b);
void activities();



//neonate.c
void handle_sigint(int sig);
void set_terminal_mode(int enable);
void neonate(char *argv[], int argc);


//signals.c
void handle_sigint_ctrlC();
void kill_all_processes();
void handle_sigtstp_ctrlZ();
void ping(char *argv[], int argc);



// fg_bg.c
void fg(char *argv[], int argc);
void bg(char *argv[], int argc);



//iMan.c
void iMan(char *argv[], int argc);



//redirection.c
void redirctn_fn(char* cmd);
void redirctn_handling_without_pipes_fg_and_bg(char * cmd);



//pipes.c
void pipe_wth_redirctn(char* pipe_cmds[], int argc_Pipes);
char* trimSpaces(char* str);
int blank_str(char* str);
int chk_invalid(char* cmd);
void handle_pipes(char* cmd);



//main.c
void printPrompt(char * user_name, char * system_name, char * currDir, char * homeDir,char prompt_path[SIZ], int cmdT_gt2_flag);
void handle_sigchld(int sig);



#endif