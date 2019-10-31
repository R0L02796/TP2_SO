#include "queue.h"
#include "lib.h"
#include "videoDriver.h"

typedef struct queue
{
    struct node * first;
    struct node * last;
    size_t size;
    size_t bytes;
    
}queue;

typedef struct node
{
    void * elem;
    struct node * next;
}node;


queue_t newQueue(size_t bytes)
{
  queue_t queue = malloc(sizeof(queue));
  if (queue == NULL) return queue;
  queue->bytes = bytes;
  queue->first = NULL;
  queue->last = NULL;
  return queue;
}

void offer(queue_t queue, void * elem) 
{
  if (queue == NULL)
  {
    putStr("queue parameter is null");
    return;
  }
  node_t newNode = malloc(sizeof(node));
  void * newData = malloc(sizeof(queue->bytes));
  if (newNode == NULL || newData == NULL)
  {
    putStr("failed to create the malloc");
    return;
  }
  memcpy(newData, elem, queue->bytes);
  newNode->next = NULL;
  newNode->elem = newData;
  if (queue->first == NULL) 
  {
    queue->first = queue->last = newNode;
  } 
  else 
  {
    queue->last->next = newNode;
    queue->last = newNode;
  }
  queue->size++;
  return ;
}

void  poll(queue_t queue, void * ret) 
{
  if (queue == NULL)
  {
    putStr("queue parameter is null");
    return;
  }  
  if (queue->first == NULL) 
  {
    putStr("queue is empty");
    return;
  }
  memcpy(ret, queue->first->elem, queue->bytes);
  node_t second = queue->first->next;
  free(queue->first->elem);
  free(queue->first);
  queue->size--;
  queue->first = second;
  return;
}


void freeQ(queue_t queue) 
{
  node_t aux;
  while (queue->first != NULL) 
  {
    aux = queue->first->next;
    free(queue->first->elem);
    free(queue->first);
    queue->first = aux;
  }
  freeQ(queue);
}


size_t size(queue_t queue)
{
  if (queue == NULL) 
    return 0;
  return queue->size;
}
