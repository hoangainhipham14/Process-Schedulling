#ifndef PROCESS_H
#define PROCESS_H

#include <stdbool.h>

typedef struct
{
    unsigned int time_arrived;
    char *process_id;
    unsigned int execution_time;
    char parallel;
    unsigned int remaining_time;
    int cpu_id;
    bool start;
    int index;
    int str_len;
} process_t;

process_t *proc_init(unsigned int time_arrived, unsigned int process_id, unsigned int execution_time, char parallel, int index);

void free_process(process_t *process);

process_t *subproc_init(unsigned int time_arrived, char *process_id, unsigned int execution_time, char parallel, int index);

#endif