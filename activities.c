#include "headers.h"
#include "global_vars.h"
#include "all_fns.h"

int compare_processes(const void* a, const void* b) {
    const Process* p_a = *(const Process**)a;
    const Process* p_b = *(const Process**)b;
    return strcmp(p_a->command, p_b->command);
}

void activities(){
    Process* p = process_list;
    int size_of_plist = 0;
    while (p) {
        size_of_plist++;
        p = p->next;
    }

    Process** processes = (Process**)malloc(size_of_plist * sizeof(Process*));

    p = process_list;
    int i = 0;
    while (p) {
        processes[i++] = p;
        p = p->next;
    }

    qsort(processes, size_of_plist, sizeof(Process*), compare_processes);

    for (i = 0; i < size_of_plist; i++) {
        Process* p = processes[i];
        printf("%d : %s - %s\n", p->pid, p->command, p->isRunning ? "Running" : "Stopped");
    }

    free(processes);
}