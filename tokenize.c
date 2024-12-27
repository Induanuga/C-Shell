#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"


void tokenize(char inp[SIZ]){

    char* cmds_SC[1500];
    int argc_SC=0;

    char* tok;
    char* ptr1;

    tok = strtok_r(inp, ";", &ptr1);

    while(tok != NULL) {

        // handle_BGP();

        cmds_SC[argc_SC] = (char*)malloc((strlen(tok) + 1) * sizeof(char));
        if (cmds_SC[argc_SC] == NULL) {
            perror("malloc");
            return;
        }
        strcpy(cmds_SC[argc_SC++], tok);


        if (strchr(cmds_SC[argc_SC - 1], '|') != NULL || 
            strchr(cmds_SC[argc_SC - 1], '<') != NULL || 
            strchr(cmds_SC[argc_SC - 1], '>') != NULL || 
            strstr(cmds_SC[argc_SC - 1], ">>") != NULL) {

            if (strchr(cmds_SC[argc_SC - 1], '|') != NULL) {
                if (strchr(cmds_SC[argc_SC - 1], '&') != NULL) {
                    char* pos = strrchr(cmds_SC[argc_SC - 1], '&');
                    if (*(pos + 1) == '\0') {
                        //chk..................
                    } 
                    else {
                        fprintf(stderr, "Error: Invalid use of '&' with pipes\n");
                    }
                } 
                else {
                    handle_pipes(cmds_SC[argc_SC - 1]);
                }
            }
            else{
                redirctn_handling_without_pipes_fg_and_bg(cmds_SC[argc_SC-1]);
            }
        }
        else{
            char* BGPs[450];
            int NoBGPs=0;
            char* FGPs[450];
            int NoFGPs=0;

            int len1=strlen(cmds_SC[argc_SC-1]);
            
            for(int i=0;i<len1;i++){
                
                char* strUse = (char*)malloc((len1 + 1) * sizeof(char));
                if (strUse == NULL) {
                    perror("malloc");
                    return;
                }
                int indUse=0;

                while(i<len1){
                    if(cmds_SC[argc_SC-1][i]=='&'){
                        strUse[indUse++]=cmds_SC[argc_SC-1][i++];
                        break;    
                    }
                    strUse[indUse++]=cmds_SC[argc_SC-1][i++];
                }
                strUse[indUse]='\0';

                if(strUse[indUse-1]=='&'){
                    //BGND process
                    strUse[indUse-1]='\0';
                    BGPs[NoBGPs] = (char *)malloc((strlen(strUse) + 1) * sizeof(char));
                    if (BGPs[NoBGPs] == NULL) {
                        perror("malloc");
                        return;
                    }
                    strcpy(BGPs[NoBGPs++],strUse);
                }
                else{
                    //FGND process
                    FGPs[NoFGPs] = (char *)malloc((strlen(strUse) + 1) * sizeof(char));
                    if (FGPs[NoFGPs] == NULL) {
                        perror("malloc");
                        return;
                    }
                    strcpy(FGPs[NoFGPs++],strUse);
                }
                free(strUse);

            }

            for(int j=0;j<NoBGPs;j++){

                // handle_BGP();

                char* temp2=BGPs[j];
                char* subtok2;
                char* ptr3;
                char* argv_BGP[100];
                int argc_BGP=0;

                subtok2=strtok_r(temp2," \t",&ptr3);
                while(subtok2!=NULL){
                    argv_BGP[argc_BGP]=(char*)malloc((strlen(subtok2)+1)*sizeof(char));
                    strcpy(argv_BGP[argc_BGP++],subtok2);
                    subtok2=strtok_r(NULL," \t",&ptr3);
                }
                argv_BGP[argc_BGP] = NULL;

                fun_BG(argv_BGP,argc_BGP);

                for (int i = 0; i < argc_BGP; i++) {
                    free(argv_BGP[i]);
                }
            }

            for(int j=0;j<NoFGPs;j++){

                // handle_BGP();

                char* temp1=FGPs[j];
                char* subtok1;
                char* ptr2;
                char* argv_FGP[256];
                int argc_FGP=0;

                subtok1=strtok_r(temp1," \t",&ptr2);
                while(subtok1!=NULL){
                    argv_FGP[argc_FGP]=(char*)malloc((strlen(subtok1)+1)*sizeof(char));
                    strcpy(argv_FGP[argc_FGP++],subtok1);
                    subtok1=strtok_r(NULL," \t",&ptr2);
                }
                argv_FGP[argc_FGP]=NULL;

                fun_FG(argv_FGP,argc_FGP);

                for (int i = 0; i < argc_FGP; i++) {
                    free(argv_FGP[i]);
                }
            }



            for (int i = 0; i < NoBGPs; i++) {
                free(BGPs[i]);
            }
            for (int i = 0; i < NoFGPs; i++) {
                free(FGPs[i]);
            }   

        }
            
        tok = strtok_r(NULL, ";", &ptr1);

        
    }


    for (int i = 0; i < argc_SC; i++) {
        free(cmds_SC[i]);
    }
        

}