#ifndef ALLOCATE_H
#define ALLOCATE_H

#include <stdbool.h>
#include "scheduler.h"

typedef struct
{
    char *filename;
    int processors;
    bool improved_scheduler;
} opts_t;

opts_t *get_opts(int argc, char **argv);
process_t **processes_arrive(char *filename, int *process_count);
void free_processes(process_t **processes, int process_count);
void perfomance_stats(process_t **processes);

#endif