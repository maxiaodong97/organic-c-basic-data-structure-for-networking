/**
 * Copyright (c) 2011 maxiaodong97@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __PT_QUEUE_H__
#define __PT_QUEUE_H__

#include "pt_defs.h"
#include "pt_node.h"

/*
 * queue 
 *
 * -----------------------------------------------------
 *  head                                 tail 
 *   |                                    |
 *   V                                    V
 * node_1 -> node_2 -> node_3 -> ... -> node_n -> NULL
 * -----------------------------------------------------
 */

typedef struct pt_queue_s
{
    pt_snode_t *head;
    pt_snode_t *tail;
} pt_queue_t;

#define PT_QUEUE_HEAD(_queue_)  ((_queue_)->head)
#define PT_QUEUE_TAIL(_queue_)  ((_queue_)->tail)

/* init a pt_queue */
static inline
void
PT_QUEUE_INIT(pt_queue_t *q)  
{
    PT_ASSERT(q != NULL);
    q->head = NULL; 
    q->tail = NULL;
}

/* add a node to the tail of the queue */
static inline
void 
PT_QUEUE_ADD_TAIL(pt_queue_t *q, pt_snode_t *n)
{
    PT_ASSERT(q != NULL);
    if (q->tail != NULL) {
        q->tail->next = n;
        q->tail = n;
        return;
    }

    PT_ASSERT(q->head == NULL);
    q->head = q->tail = n;
}

/* add a node to the head of the queue */
static inline
void 
PT_QUEUE_ADD_HEAD(pt_queue_t *q, pt_snode_t *n)
{
    PT_ASSERT(q != NULL);
    if (q->head != NULL) {
        n->next = q->head;
        q->head = n;
        return;
    }

    PT_ASSERT(q->tail == NULL);
    q->head = q->tail = n;
}

/* peek the head of the queue */
static inline 
pt_snode_t *
PT_QUEUE_PEEK_HEAD(pt_queue_t *q)
{
    return q->head;    
}

/* peek the tail of the queue */
static inline 
pt_snode_t *
PT_QUEUE_PEEK_TAIL(pt_queue_t *q)
{
    return q->tail;    
}

/* remove a node from the head of the queue */
static inline
pt_snode_t *
PT_QUEUE_REMOVE_HEAD(pt_queue_t *q)
{
    PT_ASSERT(q != NULL);    
    if (q->head != NULL) {
        pt_snode_t *n = q->head;
        q->head = n->next;
        if (q->head == NULL) {
            q->tail = NULL;    
        }
        return n;
    }
    PT_ASSERT(q->tail == NULL);
    return NULL;
}

/* remove a node from the tail of the queue */
static inline
pt_snode_t *
PT_QUEUE_REMOVE_TAIL(pt_queue_t *q)
{
    pt_snode_t *n = NULL;
    pt_snode_t *t = NULL;
    PT_ASSERT(q != NULL);    

    t = q->tail;
    n = q->head;
    
    /* case 1: > 1 node */
    while(n != NULL) {
        if (n->next == q->tail) {
            q->tail = n;
            n->next = NULL;
            return t;
        }
        n = n->next;
    }

    PT_ASSERT(q->head == q->tail);

    /* case 2: 0 node */
    if ( q->head == NULL) {
        return NULL;    
    }

    /* case 3: 1 node */
    q->head = q->tail = NULL;
    return t;
}

/* count number of nodes in a queue */
static inline
uint32_t 
PT_QUEUE_COUNT(pt_queue_t *q)
{
    uint32_t count = 0;  
    pt_snode_t *n = NULL;

    PT_ASSERT(q != NULL);
    
    n = PT_QUEUE_HEAD(q);

    while( n != NULL) {
        count++;
        n = PT_SNODE_NEXT(n);
    }
    return count;
}

#endif /* !__PT_QUEUE_H__ */
