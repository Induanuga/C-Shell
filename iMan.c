#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"


void iMan(char *argv[], int argc) {
    if (argc < 2) {
        fprintf(stderr, "Usage: iMan <command_name>\n");
        return;
    }

    char *cmd = argv[1];

    int sockfd;
    struct sockaddr_in serverAddress;
    struct hostent *svr;

    char *hostname = "man.he.net";
    int portno = 80;

    char buf[1000];
    char request[SIZ];

    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }
    
    svr = gethostbyname(hostname);
    if (svr == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)svr->h_addr_list[0], (char *)&serverAddress.sin_addr.s_addr, svr->h_length);
    serverAddress.sin_port = htons(portno);
    
    if (connect(sockfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }
    
    snprintf(request, sizeof(request),
             "GET /?topic=%s&section=all HTTP/1.1\r\n"
             "Host: man.he.net\r\n"
             "Connection: close\r\n\r\n", 
             cmd);
    
    n = write(sockfd, request, strlen(request));
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    
    int hdr_end = 0;
    int in_tag = 1;
    while ((n = read(sockfd, buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';

        if (!hdr_end) {
            char *cont_start = strstr(buf, "\n\n");
            if (cont_start != NULL) {
                hdr_end = 1;
                cont_start += 4;
                strcpy(buf, cont_start); 
            }
        }

        if (hdr_end) {
            char *p = buf;
            while (*p != '\0') {
                if (*p == '<') {
                    in_tag = 1;
                } else if (*p == '>') {
                    in_tag = 0;
                    p++;  
                    continue;
                }
                if (!in_tag) {
                    putchar(*p);
                }
                p++;
            }
        }
    }
    
    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    close(sockfd);
}
