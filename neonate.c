#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"


volatile sig_atomic_t stop = 0;

void handle_sigint(int sig) {
    stop = 1;
}

void set_terminal_mode(int enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

void neonate(char *argv[], int argc) {
    if (argc < 3 || strcmp(argv[1], "-n") != 0) {
        printf("Usage: neonate -n [time_arg]\n");
        return;
    }

    int time_arg = atoi(argv[2]);
    if (time_arg <= 0) {
        printf("Invalid time argument\n");
        return;
    }

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    set_terminal_mode(1); 

    while (!stop) {
        printf("%d\n", most_recently_created_pid);
        sleep(time_arg);
        
        struct timeval timeout;
        fd_set readfds;

        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000; 

        int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
        if (result > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
            char ch;
            read(STDIN_FILENO, &ch, 1);
            if (ch == 'x') {
                stop = 1;
            }
        }
    }

    set_terminal_mode(0); 
}
