#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "process.h"

process_t *subproc_init(unsigned int time_arrived, char *process_id, unsigned int execution_time, char parallel, int index)
{
    process_t *proc = malloc(sizeof(process_t));
    char *copy_proc_id = malloc(sizeof(char) * 13);
    strcpy(copy_proc_id, process_id);
    proc->process_id = copy_proc_id;
    proc->index = index;
    proc->execution_time = execution_time;
    proc->remaining_time = execution_time;
    return proc;
};

process_t *
proc_init(unsigned int time_arrived, unsigned int process_id, unsigned int execution_time, char parallel, int index)
{
    process_t *proc = malloc(sizeof(process_t));
    if (proc == NULL)
    {
        printf("Error! memory not allocated.");
        exit(EXIT_FAILURE);
    }

    char *str_process_id = (char *)malloc(sizeof(char) * 13);
    if (str_process_id == NULL)
    {
        printf("Error! memory not allocated.");
        exit(EXIT_FAILURE);
    }

    sprintf(str_process_id, "%d", process_id);
    proc->execution_time = execution_time;
    proc->process_id = str_process_id;
    proc->parallel = parallel;
    proc->remaining_time = execution_time;
    proc->time_arrived = time_arrived;
    proc->cpu_id = 0;
    proc->start = false;
    proc->index = index;
    proc->str_len = strlen(proc->process_id);

    return proc;
}

void free_process(process_t *process)
{
    free(process->process_id);
    free(process);
}