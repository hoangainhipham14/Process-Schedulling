#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "scheduler.h"

void execute_process(process_t *process)
{
    process->remaining_time -= 1;
};

void execution_transcript(process_t *process, int proc_remaining, int clock)
{
    if (process->start == true)
    {
        fprintf(stdout, "%d,RUNNING,pid=%s,remaining_time=%d,cpu=%d\n", clock, process->process_id, process->remaining_time, process->cpu_id);
    }
    else
    {
        fprintf(stdout, "%d,FINISHED,pid=%s,proc_remaining=%d\n", clock, process->process_id, proc_remaining);
    }
}

scheduler_t *sched_init()
{
    scheduler_t *sched = malloc(sizeof(scheduler_t));
    if (sched == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    sched->proc_remaining = 0;
    sched->empty = true;
    sched->interrupt = false;
    sched->curr = NULL;
    sched->remain_t = 0;
    return sched;
};

process_t *incoming_proc(node_t **queue, process_t *curr_proc, process_t *new_proc, scheduler_t **sched)
{
    if (((new_proc->remaining_time == curr_proc->remaining_time) && (atoi(new_proc->process_id) < atoi(curr_proc->process_id))) || (new_proc->remaining_time < curr_proc->remaining_time))
    {
        queue_push(curr_proc, queue, curr_proc->str_len);
        sched_remove(*sched, curr_proc);
        sched_add(*sched, new_proc);
        (*sched)->interrupt = true;
        return new_proc;
    }
    else
    {
        queue_push(new_proc, queue, new_proc->str_len);
        return curr_proc;
    }
}

// void perfomance_stats(process_t **processes, int makespan)
// {
//     int turnaround_time;
//     int ave_time_overhead;
//     int max_time_overhead;
//     fprintf(stdout, "Turnaround time %d", turnaround_time);
//     fprintf(stdout, "Time overhead %d %d", max_time_overhead, ave_time_overhead);
//     fprintf(stdout, "Makespan %d", makespan);
// }

void sched_proc(process_t **processes, int process_count, int processors)
{
    queue_t **queue = (queue_t **)malloc(sizeof(queue_t *) * processors);
    scheduler_t **sched = (scheduler_t **)malloc(sizeof(scheduler_t *) * processors);
    if (queue == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    if (sched == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < processors; i++)
    {
        queue[i] = queue_init();
        sched[i] = sched_init();
    }

    process_t **curr_proc = (process_t **)malloc(sizeof(process_t *) * processors);
    if (curr_proc == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    int *num_finished_subproc = (int *)malloc(sizeof(int) * process_count);
    if (curr_proc == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    node_t *concurr_proc = NULL;
    process_t *new_proc;
    int clock = 0;
    int assigned_CPU = 0;
    int num_concurr_proc = 0;
    bool alr_sched = false;
    int proc_remaining = 0;
    int num_proc_left = process_count;
    int min_remain_time;

    while (num_proc_left > 0)
    {
        num_concurr_proc = 0;
        for (int i = 0; i < process_count; i++)
        {
            if (processes[i]->time_arrived == clock)
            {
                num_finished_subproc[i] = 0;
                if (processes[i]->parallel == 'p')
                {
                    create_subprocesses(processes[i], processors, &concurr_proc, processes[i]->str_len);
                    num_concurr_proc += processors;
                }
                else
                {
                    queue_push(processes[i], &concurr_proc, processes[i]->str_len);
                    num_concurr_proc += 1;
                }
                proc_remaining += 1;
            }
        }

        for (int i = 0; i < num_concurr_proc; i++)
        {
            alr_sched = false;
            new_proc = queue_pop(&concurr_proc);
            if (strstr(new_proc->process_id, ".") != NULL)
            {
                char *curr_proc_index = (char *)malloc(sizeof(char) * 2);
                if (curr_proc_index == NULL)
                {
                    perror("Error allocating memory");
                    exit(EXIT_FAILURE);
                }
                memcpy(curr_proc_index, &((new_proc->process_id)[new_proc->str_len + 1]), 2);
                if (sched[atoi(curr_proc_index)]->empty == true)
                {
                    assigned_CPU = atoi(curr_proc_index);
                    curr_proc[assigned_CPU] = new_proc;
                    curr_proc[assigned_CPU]->start = true;
                    sched_add(sched[assigned_CPU], curr_proc[assigned_CPU]);
                    alr_sched = true;
                }
                if (alr_sched == false)
                {
                    assigned_CPU = atoi(curr_proc_index);
                    curr_proc[assigned_CPU] = incoming_proc(&(queue[assigned_CPU]->lst), curr_proc[assigned_CPU], new_proc, &sched[assigned_CPU]);
                    if (sched[assigned_CPU]->interrupt == true)
                    {
                        curr_proc[assigned_CPU]->start = true;
                        sched[assigned_CPU]->interrupt = false;
                    }
                }
                free(curr_proc_index);
            }
            else
            {
                for (int j = 0; j < processors; j++)
                {

                    // if not, schedule new process
                    if (sched[j]->empty == true)
                    {
                        assigned_CPU = j;
                        curr_proc[assigned_CPU] = new_proc;
                        curr_proc[assigned_CPU]->start = true;
                        sched_add(sched[assigned_CPU], curr_proc[assigned_CPU]);
                        alr_sched = true;
                        break;
                    }
                }

                if (alr_sched == false)
                {
                    min_remain_time = sched[0]->remain_t;
                    assigned_CPU = 0;
                    for (int i = 0; i < processors; i++)
                    {
                        if (min_remain_time > sched[i]->remain_t)
                        {
                            assigned_CPU = i;
                            min_remain_time = sched[i]->remain_t;
                        }
                    }

                    // decide if postpone and add current running process to queue
                    curr_proc[assigned_CPU] = incoming_proc(&(queue[assigned_CPU]->lst), curr_proc[assigned_CPU], new_proc, &sched[assigned_CPU]);
                    if (sched[assigned_CPU]->interrupt == true)
                    {
                        curr_proc[assigned_CPU]->start = true;
                        sched[assigned_CPU]->interrupt = false;
                    }
                }
            }
            sched[assigned_CPU]->remain_t += new_proc->execution_time;
            sched[assigned_CPU]->proc_remaining += 1;
        }

        for (int i = 0; i < processors; i++)
        {
            // Execution of a process starts/ resumes
            if (sched[i]->empty == false && curr_proc[i]->start == true)
            {
                curr_proc[i]->cpu_id = i;
                execution_transcript(curr_proc[i], process_count, clock);
                curr_proc[i]->start = false;
            }

            if (sched[i]->empty == false)
            {
                execute_process(curr_proc[i]);
                sched[i]->remain_t -= 1;
            }
        }

        clock += 1;

        int num_finished_proc = 0;
        for (int i = 0; i < processors; i++)
        {
            if (sched[i]->empty == false && curr_proc[i]->remaining_time == 0)
            {
                if (strstr(curr_proc[i]->process_id, ".") != NULL)
                {
                    num_finished_subproc[curr_proc[i]->index] += 1;
                    if (num_finished_subproc[curr_proc[i]->index] == processors)
                    {
                        num_finished_proc += 1;
                        proc_remaining -= 1;
                    }
                }
                else
                {
                    num_finished_proc += 1;
                    proc_remaining -= 1;
                }
            }
        }

        for (int i = 0; i < processors; i++)
        {
            if (sched[i]->empty == false && curr_proc[i]->remaining_time == 0)
            {
                sched_remove(sched[i], curr_proc[i]);
                sched[i]->proc_remaining -= 1;
                if (strstr(curr_proc[i]->process_id, ".") != NULL && num_finished_subproc[curr_proc[i]->index] == processors)
                {
                    num_finished_subproc[curr_proc[i]->index] = 0;
                    processes[curr_proc[i]->index]->process_id = processes[curr_proc[i]->index]->process_id;
                    execution_transcript(processes[curr_proc[i]->index], proc_remaining, clock);
                }

                if (strstr(curr_proc[i]->process_id, ".") == NULL)
                {
                    execution_transcript(curr_proc[i], proc_remaining, clock);
                }

                if (sched[i]->proc_remaining > 0)
                {
                    curr_proc[i] = queue_pop(&(queue[i]->lst));
                    curr_proc[i]->start = true;
                    sched_add(sched[i], curr_proc[i]);
                }
            }
        }

        num_proc_left -= num_finished_proc;
    }

    for (int i = 0; i < processors; i++)
    {
        free(queue[i]);
        free(sched[i]);
    }

    free(sched);
    free(queue);
    free(num_finished_subproc);
    free(curr_proc);
};

void create_subprocesses(process_t *proc, int processors, node_t **concurr_proc, int str_len)
{
    // calculate k
    int k = 1;
    while (((proc->execution_time / k) >= 1) && k < processors)
    {
        k += 1;
    }
    if ((proc->execution_time % k) > 0)
    {
        proc->execution_time = ceil(proc->execution_time / k) + 2;
        proc->remaining_time = ceil(proc->execution_time / k) + 2;
    }
    else
    {
        proc->execution_time = proc->execution_time / k + 1;
        proc->remaining_time = proc->execution_time / k + 1;
    }

    for (int i = 0; i < processors; i++)
    {
        // Include null char
        char *proc_id = (char *)malloc(sizeof(char) * 13);
        if (proc_id == NULL)
        {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }
        strcpy(proc_id, proc->process_id);
        strcat(proc_id, ".");
        char *index = (char *)malloc(sizeof(char));
        if (index == NULL)
        {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }
        sprintf(index, "%d", i);
        strcat(proc_id, index);
        process_t *subproc = subproc_init(proc->time_arrived, proc_id, proc->execution_time, 'p', proc->index);
        subproc->str_len = strlen(proc->process_id);
        queue_push(subproc, concurr_proc, str_len);
        free(proc_id);
        free(index);
    }
}

void sched_remove(scheduler_t *sched, process_t *finished_proc)
{
    free(sched->curr);
    sched->empty = true;
}

void sched_add(scheduler_t *sched, process_t *ready_proc)
{
    sched->curr = (node_t *)malloc(sizeof(node_t *));
    if (sched->curr == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    sched->curr->next = NULL;
    sched->curr->process = ready_proc;
    sched->empty = false;
}
