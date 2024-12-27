#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"


void printFileCont(char *path) {
    FILE *file = fopen(path, "r");
    if (file==NULL) {
        printf("Missing permissions for task!\n");
        return;
    }
    char buff[1024];
    while (fgets(buff, sizeof(buff), file)) {
        printf("%s", buff);
    }
    fclose(file);
}


void changeDir(char* path){
    strcpy(prevDir, currDir);
    char ep[SIZ];
    if (strncmp(path, "~/", 2) == 0) {
        snprintf(ep, SIZ, "%s%s", homeDir, path + 1);
    } else {
        strncpy(ep, path, SIZ - 1);
    }
    char res[SIZ];
    if (realpath(ep, res) == NULL) {
        perror("realpath");
        return;
    }
    strncpy(currDir, res, SIZ - 1);
    handling_prompt();
    if (chdir(path)!= 0) {
        printf("Missing permissions for task!\n");
    }
}



void searchDir(char* search, char* basePath, char* currPath, int f_flag, int d_flag, int e_flag, int * No_ofFD_fnd, char*  one_match_fp){
    char res[SIZ];
    if (realpath(currPath, res) == NULL) {
        perror("realpath");
        return;
    }

    DIR* d = opendir(res);
    if(d==NULL){
        perror("opendir");
        return;
    }

    struct dirent* entry;

    char fp[SIZ];
    char rp[SIZ];

    while((entry=readdir(d))!=NULL){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        if (snprintf(fp, sizeof(fp), "%s/%s", res, entry->d_name) >= sizeof(fp)) {
            fprintf(stderr, "Error: Buffer overflow in snprintf.\n");
            return;
        }
        
        if(strcmp(basePath,".")==0){
            char temp1[SIZ];
            if (realpath(basePath, temp1) == NULL) {
                perror("realpath");
                return;
            }
            snprintf(rp, sizeof(rp), ".%s/%s", currPath + strlen(temp1), entry->d_name);
        }
        else{
            if (strncmp(currPath, basePath, strlen(basePath)) == 0) {
                snprintf(rp, sizeof(rp), ".%s/%s", currPath + strlen(basePath), entry->d_name);
            }
            else{
                snprintf(rp, sizeof(rp), "%s/%s", currPath, entry->d_name);
            }
        }

        struct stat st;
        if(stat(fp, &st)!=0){
            perror("stat");
            return;
        }

        if(S_ISDIR(st.st_mode)){
            if (strncmp(entry->d_name, search, strlen(search)) == 0 && d_flag) {
                printf(BLUE_COLOR "%s" ORG_COLOR "\n", rp);
                (*No_ofFD_fnd)++;
                if (e_flag && *No_ofFD_fnd == 1) {
                    strncpy(one_match_fp, fp, SIZ - 1);
                }
            }

            searchDir(search, basePath, fp, f_flag, d_flag, e_flag, No_ofFD_fnd, one_match_fp);
        }
        else if(S_ISREG(st.st_mode)){
            if (strncmp(entry->d_name, search, strlen(search)) == 0 && f_flag) {
                printf(GREEN_COLOR "%s" ORG_COLOR "\n", rp);
                (*No_ofFD_fnd)++;
                if (e_flag && *No_ofFD_fnd == 1) {
                    strncpy(one_match_fp, fp, SIZ - 1);
                }
            }
        }
    }
    closedir(d);
}



void seek(char *argv[], int argc) {

    int f_flag=0;
    int d_flag=0;
    int e_flag=0;
    char* search=NULL;
    char tar_dir[SIZ]=".";
    int idx;
    for(idx=1;idx<argc && argv[idx][0]=='-';idx++){
        if(strcmp(argv[idx],"-d")==0){
            d_flag=1;
        }
        else if(strcmp(argv[idx],"-f")==0){
            f_flag=1;
        }
        else if(strcmp(argv[idx],"-e")==0){
            e_flag=1;
        }
    }

    if(idx<argc){
        search=argv[idx++];
    }
    else{
        fprintf(stderr, "Search name is missing\n");
        return;
    }

    if(idx<argc){
        if (strcmp(argv[idx], "~") == 0) {
            snprintf(tar_dir, SIZ, "%s", homeDir);
        } 
        else if(strncmp(argv[idx],"~/",2)==0){
            snprintf(tar_dir,SIZ,"%s%s",homeDir,argv[idx]+1);
        }
        else{
            strncpy(tar_dir,argv[idx],SIZ);
        }
    }

    if(f_flag && d_flag){
        fprintf(stderr, "Invalid flags!\n");
        return;
    }

    if (f_flag == 0 && d_flag == 0) {
        f_flag = 1;
        d_flag = 1;
    }

    int No_ofFD_fnd=0;
    char one_match_fp[SIZ];

    searchDir(search, tar_dir, tar_dir, f_flag, d_flag, e_flag, &No_ofFD_fnd, one_match_fp);

    if(No_ofFD_fnd==0){
        printf("No match found!\n");
    }
    else if(No_ofFD_fnd==1 && e_flag){
        struct stat st;
        if(stat(one_match_fp, &st)!=0){
            perror("stat");
            return;
        }

        if(S_ISDIR(st.st_mode)){
            changeDir(one_match_fp);
        }
        else if(S_ISREG(st.st_mode)){
            printFileCont(one_match_fp);
        }
    }
}