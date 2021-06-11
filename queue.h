#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"

typedef struct node node_t;
struct node
{
    node_t *next;
    process_t *process;
};

typedef struct
{
    node_t *lst;
    int CPU_num;
} queue_t;

queue_t *queue_init();
void queue_push(process_t *process, node_t **lst, int str_len);
process_t *queue_pop(node_t **queue);
// node_t *lst_init(process_t *process);
#endif