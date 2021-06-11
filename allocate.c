#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "allocate.h"

/* * * * * * * * * * * MAIN FUNCTION * * * * * * * * * * */
int main(int argc, char **argv)
{
    int *process_count = malloc(sizeof(int));
    if (process_count == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    *process_count = 0;

    opts_t *opts = get_opts(argc, argv);

    process_t **processes = processes_arrive(opts->filename, process_count);

    sched_proc(processes, *process_count, opts->processors);

    free_processes(processes, *process_count);
    free(process_count);
    free(opts);
}

/* * * * * * * * * * * HELPER FUNCTION CODE * * * * * * * * * * */
opts_t *get_opts(int argc, char **argv)
{
    opts_t *opts = malloc(sizeof(opts_t));
    if (opts == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    char input;
    while ((input = getopt(argc, argv, "f:p:c")) != EOF)
    {
        switch (input)
        {
        case 'f':
            opts->filename = optarg;
            break;
        case 'p':
            opts->processors = atoi(optarg);
            break;
        case 'c':
            opts->improved_scheduler = true;
            break;
        default:
            break;
        }
    }

    return opts;
}

process_t **processes_arrive(char *filename, int *process_count)
{

    FILE *fp;
    if (!(fp = fopen(filename, "r")))
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int time_arrived;
    int process_id;
    int execution_time;
    char parallel;
    int max_processeses = 1;
    process_t **processes = (process_t **)malloc(sizeof(process_t *) * (max_processeses + 1));
    if (processes == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fp, "%d %d %d %c", &time_arrived, &process_id, &execution_time, &parallel) == 4)
    {

        if (*process_count >= max_processeses)
        {
            max_processeses *= 2;
            processes = realloc(processes, sizeof(process_t *) * (max_processeses + 1));
            if (processes == NULL)
            {
                perror("Error allocating memory");
                exit(EXIT_FAILURE);
            }
        }

        processes[*process_count] = proc_init(time_arrived,
                                              process_id, execution_time, parallel, *process_count);

        *process_count += 1;
    };

    processes[*process_count] = NULL;

    fclose(fp);

    return processes;
}

void free_processes(process_t **processes, int process_count)
{

    for (int i = 0; i < process_count; i++)
    {
        free(processes[i]);
    }

    free(processes);
}
