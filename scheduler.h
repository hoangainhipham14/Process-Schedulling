#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include "process.h"
#include "queue.h"

typedef struct
{
    int proc_remaining;
    bool empty;
    bool interrupt;
    node_t *curr;
    int remain_t;
} scheduler_t;

scheduler_t *sched_init();
void sched_proc(process_t **processes, int process_count, int processors);
void sched_remove(scheduler_t *sched, process_t *finished_proc);
void sched_add(scheduler_t *sched, process_t *ready_proc);
process_t *incoming_proc(node_t **queue, process_t *curr_proc, process_t *new_proc, scheduler_t **sched);
void execute_process(process_t *process);
void execution_transcript(process_t *process, int proc_remaining, int clock);
void create_subprocesses(process_t *proc, int processors, node_t **concurr_proc, int str_len);
#endif