#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"


void printPrompt(char * user_name, char * system_name, char * currDir, char * homeDir,char prompt_path[SIZ], int cmdT_gt2_flag){

    if (dup2(saved_stdin, STDIN_FILENO) == -1) {
        perror("dup2");
        return;
    }
    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        perror("dup2");
        return;
    }

    char original_prompt_path[SIZ];
    if(cmdT_gt2_flag==1){
        strcpy(original_prompt_path, prompt_path);
        strcat(prompt_path, cmd_gt2_add);
        cmdT_gt2_flag = 0;
        strcpy(cmd_gt2_add, "");
    }
    printf(PINK_COLOR "<" DARK_SEA_GREEN_COLOR "%s" ORG_COLOR "@" LIGHT_CYAN_COLOR "%s" ORG_COLOR ":" YELLOW_COLOR "%s" PINK_COLOR "> " ORG_COLOR ,user_name,system_name,prompt_path);
    if (cmdT_gt2_flag==0) {
        strcpy(prompt_path, original_prompt_path);
        handling_prompt();
    }
}



void handle_sigchld(int sig) {
    int saved_errno = errno; 
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {

        if (WIFEXITED(status)) {
            remove_process(pid);
            printf("Process with PID %d exited with status %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            remove_process(pid);
            printf("Process with PID %d was killed by signal %d\n", pid, WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {  
            update_process_state(pid, 0);
            printf("Process with PID %d was stopped by signal %d\n", pid, WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            update_process_state(pid, 1);
            printf("Process with PID %d continued\n", pid);
        }
    }
    errno = saved_errno;
}


int main(){
    char pwd[SIZ];
    if(getcwd(pwd,sizeof(pwd))==NULL){
        perror("getcwd");
        return 1;
    }
    strcpy(homeDir,pwd);
    strcpy(currDir,pwd);

    uid_t user_id = getuid();

    struct passwd * det = getpwuid(user_id);
    char user_name[SIZ];
    if(det!=NULL){
        strcpy(user_name,det->pw_name);
    }
    else{
        perror("getpwuid");
        return 1;
    }

    char system_name[SIZ];
    if(gethostname(system_name,sizeof(system_name))!=0){
        perror("gethostname");
        return 1;
    }

    strcpy(prompt_path,"~");

    PID_SHELL=getpid();

    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin == -1) {
        perror("dup");
        return 1;
    }

    saved_stdout=dup(STDOUT_FILENO);
    if(saved_stdout==-1){
        perror("dup");
        return 1;
    }

    signal(SIGINT, handle_sigint_ctrlC);
    signal(SIGTSTP, handle_sigtstp_ctrlZ);


    // load_myshrc();


    int fd = open("log_file.txt", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);

    load_existing_commands();


    struct sigaction sa;
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART ;
    // | SA_NOCLDSTOP
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }


    while(1){

        // handle_BGP();
        
        printPrompt(user_name,system_name,currDir,homeDir,prompt_path,cmdT_gt2_flag);

        char inp[SIZ];

        if (fgets(inp, sizeof(inp), stdin) != NULL) {
            inp[strcspn(inp, "\n")] = '\0'; 
            if (strlen(inp) == 0) {
                continue;
            }

            char store_1[SIZ];
            strcpy(store_1,inp);

            tokenize(inp);

            log_help1(store_1);
        } 
        else {
            if (feof(stdin)) {
                kill_all_processes();
                printf("\nExiting shell...\n");
                break;
            } 
            else {
                perror("fgets");
            }
        }

    }

    return 0;
}
