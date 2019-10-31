#ifndef QUEUE_H
#define QUEUE_h

typedef struct node * node_t;
typedef struct queue * queue_t;



queue_t newQueue(size_t bytes);

//add at the end
void offer(queue_t queue, void * elem); 
//returns the first in ret
void  poll(queue_t queue, void * ret); 

void freeQ(queue_t queue); 

size_t sizeQ(queue_t queue);


#endif