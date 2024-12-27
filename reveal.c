#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"

int compare(const void * a, const void * b){
    return strcmp(*(const char **)a,*(const char **)b);
}

void chkPermissions(mode_t mode, char *str) {
    if (S_ISDIR(mode)) 
    str[0] = 'd';
    if (S_ISLNK(mode)) 
    str[0] = 'l';

    if (mode & S_IRUSR) 
    str[1] = 'r';
    if (mode & S_IWUSR) 
    str[2] = 'w';
    if (mode & S_IXUSR) 
    str[3] = 'x';

    if (mode & S_IRGRP) 
    str[4] = 'r';
    if (mode & S_IWGRP) 
    str[5] = 'w';
    if (mode & S_IXGRP) 
    str[6] = 'x';

    if (mode & S_IROTH) 
    str[7] = 'r';
    if (mode & S_IWOTH) 
    str[8] = 'w';
    if (mode & S_IXOTH) 
    str[9] = 'x';
}



void showFandD(char *path, int a_flag, int l_flag) {
    struct dirent *entry;
    DIR *d;
    d = opendir(path);
    if (d == NULL) {
        perror("opendir");
        return;
    }

    char * all[1024];
    int cnt = 0;

    blkcnt_t total_blocks = 0;
    
    while ((entry = readdir(d)) != NULL) {
        if(a_flag==1){
            all[cnt++] = strdup(entry->d_name);
        }
        else{
            if(entry->d_name[0] != '.'){
                all[cnt++] = strdup(entry->d_name);
            }
        }
    }
    closedir(d);

    qsort(all, cnt, sizeof(char *), compare);

    for (int i = 0; i < cnt; i++) {
        struct stat st;

        char fpath[1024];
        snprintf(fpath, sizeof(fpath), "%s/%s", path, all[i]);

        if (stat(fpath, &st) == 0) {
            total_blocks += st.st_blocks; 
        } 
        else {
            perror("stat");
        }
    }

    if (l_flag == 1) {
        printf("total %ld\n", total_blocks / 2);
    }

    for (int i = 0; i < cnt; i++) {
        struct stat st;

        char fpath[1024];
        snprintf(fpath, sizeof(fpath), "%s/%s", path, all[i]);

        if (stat(fpath, &st) == 0) {
            if (l_flag==1) {
                char permissions[11];
                strcpy(permissions, "----------");
                chkPermissions(st.st_mode, permissions);

                struct passwd *usr_det = getpwuid(getuid());
                struct group *grp_det = getgrgid(getgid());

                char Time[25];
                strftime(Time, sizeof(Time), "%b %d %H:%M", localtime(&st.st_mtime));

                printf("%s %3ld %s %s %9ld %s ",permissions, (long)st.st_nlink, usr_det->pw_name, grp_det->gr_name, (long)st.st_size, Time);
            }

            if (S_ISDIR(st.st_mode)) {
                printf(BLUE_COLOR "%s" ORG_COLOR "\n", all[i]);
            } 
            else if (st.st_mode & S_IXUSR) {
                printf(GREEN_COLOR "%s" ORG_COLOR "\n", all[i]);
            }
            else {
                printf(WHITE_COLOR "%s" ORG_COLOR "\n", all[i]);
            }
        } 
        else {
            perror("stat");
        }

        free(all[i]);
    }
}




void reveal(char * argv[], int argc){

    if(strcmp(argv[0],"reveal")==0){
        int a_flag=0;
        int l_flag=0;

        int pathGiven = 0;

        for(int i=1;i<argc;i++){
            if(strcmp(argv[i],"-")==0){
                showFandD(prevDir,a_flag,l_flag);
                return;
            }
            else if(argv[i][0]=='-'){
                for(int j=1;argv[i][j]!='\0';j++){
                    if(argv[i][j]=='a'){
                        a_flag=1;
                    }
                    if(argv[i][j]=='l'){
                        l_flag=1;
                    }
                }
            }
            else{
                pathGiven = 1;
                if(strcmp(argv[i],"~")==0){
                    showFandD(homeDir,a_flag,l_flag);
                    return;
                }
                char ep[SIZ];
                if(strncmp(argv[i],"~/",2)==0){
                    snprintf(ep, SIZ, "%s%s", homeDir, argv[i] + 1);
                }
                else{
                    strcpy(ep,argv[i]);
                }
                
                char res[SIZ];
                if(realpath(ep,res)==NULL){
                    perror("realpath");
                    return;
                }
                showFandD(res,a_flag,l_flag);
                return;
            }
        }

        if (!pathGiven) {
            showFandD(".", a_flag, l_flag);
            return;
        }
    }
}
