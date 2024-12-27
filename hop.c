#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"


int readPermission(char *fp){
    struct stat fdet;
    if(stat(fp,&fdet)!=0){
        perror("stat");
        return 0;
    }
    return (fdet.st_mode & S_IRUSR);
}

int validDir(char * fp){
    struct stat fdet;
    if(stat(fp,&fdet)!=0){
        perror("stat");
        return 0;
    }
    return S_ISDIR(fdet.st_mode);
}

void handling_prompt(){
    if(strncmp(currDir,homeDir,strlen(homeDir))==0){
        //inside the home dir -> so prompt_path should be relative
        if (strlen(currDir) == strlen(homeDir)) {
            strcpy(prompt_path, "~");
        } 
        else {
            char temp[SIZ];
            strcpy(temp, currDir + strlen(homeDir));
            snprintf(prompt_path, SIZ+1, "~%s", temp);
        }
    }
    else{
        //outside the home dir -> so prompt_path should be absolute
        strcpy(prompt_path, currDir);
    }
}

void hop_ntg_or_tilde(){
    strcpy(prevDir,currDir);
    strcpy(currDir,homeDir);
    if (chdir(homeDir) != 0) {
        perror("chdir");
        return;
    }
    strcpy(prompt_path,"~");
    printf("%s\n",currDir);
}

void hop_hyphen(){
    if (prevDir[0] == '\0') {
        printf("OLDPWD not set\n");
        return;
    }
    char tempDir[SIZ];
    strcpy(tempDir, currDir);
    if (chdir(prevDir) != 0) {
        perror("chdir");
        return;
    }
    strcpy(currDir, prevDir);
    strcpy(prevDir, tempDir);
    handling_prompt();
    printf("%s\n", currDir);
}


void hop_specific(char *path) {
    if(strcmp(path,"~")==0){
        hop_ntg_or_tilde();
        return;
    }
    if(strcmp(path,"-")==0){
        hop_hyphen();
        return;
    }
    strcpy(prevDir,currDir);
    char ep[SIZ];
    if(strncmp(path,"~/",2)==0){
        snprintf(ep, SIZ, "%s%s", homeDir, path + 1);
    }
    else{
        strcpy(ep,path);
    }
    
    char res[SIZ];
    if(realpath(ep,res)==NULL){
        perror("realpath");
        return;
    }

    if (validDir(res)) {
        if (chdir(res) != 0) {
            perror("chdir");
            return;
        }
        strcpy(currDir, res);
        handling_prompt();
        printf("%s\n", currDir);
    } 
    else {
        printf("Error: Not a valid directory: %s\n", res);
    }
}

void hop(char *argv[], int argc) {

    if (argc == 1 && strcmp(argv[0], "hop") == 0) {
        hop_ntg_or_tilde();
    } 
    else if (argc >= 2 && strcmp(argv[0], "hop") == 0) {
        for (int i = 1; i < argc; i++) {
            hop_specific(argv[i]);
        }
    }
}



/*
1.cd path or cd path/
2.cd /home/user/Documents/path or cd /home/user/Documents/path/
3.Using parent directory ex: cd ../../path or cd ../../path/
4.Using current directory ex: cd ./path or ./path/
5.Home directory : cd ~/path or cd ~/path/ 

1.cd path
2.cd /home/user/Documents/path
3.cd ../../path
4.cd ./path
5.cd ~/path
*/