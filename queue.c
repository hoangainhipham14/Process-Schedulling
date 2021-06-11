#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

queue_t *queue_init()
{
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    queue->lst = NULL;
    return queue;
}

void queue_push(process_t *process, node_t **lst, int str_len)
{

    if ((*lst) == NULL)
    {
        (*lst) = (node_t *)malloc(sizeof(node_t));
        (*lst)->next = NULL;
        (*lst)->process = process;
        return;
    }
    node_t *curr = (*lst);
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    char *proc_index = (char *)malloc(sizeof(char) * 2);
    char *proc_id = (char *)malloc(sizeof(char) * 2);
    memcpy(proc_index, &((process->process_id)[str_len + 1]), 2);
    memcpy(proc_id, &((process->process_id)[0]), str_len);

    if (strstr(process->process_id, ".") != NULL)
    {
        if (strstr(curr->process->process_id, ".") != NULL)
        {
            char *curr_proc_index = (char *)malloc(sizeof(char) * 2);
            memcpy(curr_proc_index, &((curr->process->process_id)[str_len + 1]), 2);
            if (atoi(curr_proc_index) > atoi(proc_index))
            {

                new_node->process = process;
                new_node->next = curr;
                (*lst) = new_node;
                return;
            }
        }
        else
        {
            if (curr->process->remaining_time > process->remaining_time || ((curr->process->remaining_time == process->remaining_time) && (atoi(curr->process->process_id) > atoi(proc_id))))
            {
                new_node->process = process;
                new_node->next = curr;
                (*lst) = new_node;
                return;
            }
        }
    }
    else
    {
        if (curr->process->remaining_time > process->remaining_time || ((curr->process->remaining_time == process->remaining_time) && (atoi(curr->process->process_id) > atoi(process->process_id))))
        {
            new_node->process = process;
            new_node->next = curr;
            (*lst) = new_node;
            return;
        }
    }

    while (curr->next != NULL)
    {
        node_t *prev = curr;
        curr = curr->next;
        if (strstr(process->process_id, ".") != NULL)
        {
            if (strstr(curr->process->process_id, ".") != NULL)
            {
                char *curr_proc_index = (char *)malloc(sizeof(char) * 2);
                memcpy(curr_proc_index, &((curr->process->process_id)[str_len + 1]), 2);
                if (atoi(curr_proc_index) > atoi(proc_index))
                {

                    new_node->process = process;
                    new_node->next = curr;
                    prev->next = new_node;
                    return;
                }
            }
            else
            {
                if (curr->process->remaining_time > process->remaining_time || ((curr->process->remaining_time == process->remaining_time) && (atoi(curr->process->process_id) > atoi(proc_id))))
                {

                    new_node->process = process;
                    new_node->next = curr;
                    prev->next = new_node;
                    return;
                }
            }
        }
        else
        {
            if (curr->process->remaining_time > process->remaining_time || ((curr->process->remaining_time == process->remaining_time) && (atoi(curr->process->process_id) > atoi(process->process_id))))
            {

                new_node->process = process;
                new_node->next = curr;
                prev->next = new_node;
                return;
            }
        }
    }

    if (strstr(process->process_id, ".") != NULL)
    {
        if (strstr(curr->process->process_id, ".") != NULL)
        {
            char *curr_proc_index = (char *)malloc(sizeof(char) * 2);
            memcpy(curr_proc_index, &((curr->process->process_id)[str_len + 1]), 2);
            if (atoi(curr_proc_index) < atoi(proc_index))
            {

                new_node->process = process;
                new_node->next = NULL;
                curr->next = new_node;
                return;
            }
        }
        else
        {
            if (curr->process->remaining_time < process->remaining_time || ((curr->process->remaining_time == process->remaining_time) && (atoi(curr->process->process_id) < atoi(proc_id))))
            {

                new_node->process = process;
                new_node->next = NULL;
                curr->next = new_node;
                return;
            }
        }
    }
    else
    {
        if (curr->process->remaining_time < process->remaining_time || ((curr->process->remaining_time == process->remaining_time) && (atoi(curr->process->process_id) < atoi(process->process_id))))
        {
            new_node->process = process;
            new_node->next = NULL;
            curr->next = new_node;
            return;
        }
    }
}

process_t *queue_pop(node_t **queue)
{
    node_t *new_head = (*queue)->next;
    process_t *process = (*queue)->process;
    free(*queue);
    *queue = new_head;
    return process;
}