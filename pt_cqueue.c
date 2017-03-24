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
#include "pt_cqueue.h"

/*
 * start the calendar queue from "now", must be called before add any object or
 * node to the calendar queue.
 */
static
void 
start(struct pt_cqueue_s *cq, pt_time_t now)
{
   cq->elapse_time = now;
}

/*
 * Poll a cqueue node from a calendar queue that its timeout needs to be 
 * triggered, the node will be dequeued from the calendar queue, caller need to
 * call ->add_node to insert it back or free it. caller should poll it until 
 * NULL is returned.
 */
static
pt_cqueue_node_t* 
poll_next(struct pt_cqueue_s *cq, pt_time_t cur_time)
{
    while (PT_TIME_GE(cur_time, cq->elapse_time)) {
        pt_dlist_t *dl = &(cq->buckets[cq->current]);
        pt_dnode_t *node = PT_DLIST_HEAD(dl);
        if (node != NULL) {
            if (PT_TIME_LE(((pt_cqueue_node_t*)node)->timeout, cur_time)) {
                return (pt_cqueue_node_t*)PT_DLIST_REMOVE_NODE(dl, node); 
            }
        }
        cq->current++;
        cq->current = cq->current % cq->nbuckets;
        cq->elapse_time += cq->delta_time;
    }

    return NULL;
}

/*
 * add a cqueue_node back to calendar queue, caller can adjuct timeout value in
 * the node before insert it to the queue
 */
static 
void 
add_node(struct pt_cqueue_s *cq,
    pt_cqueue_node_t *cq_node)
{
    uint32_t index = 0;
    pt_dlist_t *dl = NULL; 
    pt_dnode_t *node = NULL;

    PT_ASSERT(PT_TIME_GE(cq_node->timeout, cq->elapse_time));

    index = (uint32_t)(cq_node->timeout - cq->elapse_time) / cq->delta_time;
    index += cq->current;
    index = index % cq->nbuckets;
    cq_node->bucket_index = index;
    dl = &(cq->buckets[index]);
    node = PT_DLIST_HEAD(dl);

    PT_DNODE_INIT(&(cq_node->node));
    while(node != NULL) {
        if (PT_TIME_GT(((pt_cqueue_node_t*)node)->timeout, cq_node->timeout)) {
            PT_DLIST_ADD_BEFORE(dl, node, &(cq_node->node));   
            return;
        } 
        node = PT_DNODE_NEXT(node);
    }
    PT_DLIST_ADD_TAIL(dl, &(cq_node->node));
}

/*
 * remove a cqueue_node from calendar queue
 */
static 
void 
remove_node(struct pt_cqueue_s *cq,
    pt_cqueue_node_t *cq_node)
{
    pt_dlist_t *dl = NULL; 

    dl = &(cq->buckets[cq_node->bucket_index]);
    PT_DLIST_REMOVE_NODE(dl, &(cq_node->node)); 
    PT_FREE(cq_node);
}

/*
 * add a opaque object pointer which will be timeouted at "abs_time"  
 */
static pt_cqueue_node_t* 
add_object(struct pt_cqueue_s *cq, void *object, pt_time_t abs_time)
{
    pt_cqueue_node_t *cq_node = NULL;

    cq_node = PT_CALLOC(1, sizeof(pt_cqueue_node_t));
    if (cq_node == NULL) {
        return NULL;    
    }

    cq_node->ptr = object;
    cq_node->timeout = abs_time;
    cq_node->cq = cq;

    add_node(cq, cq_node);
    return cq_node;
}

/* 
 * Create a calendar queue with nbuckets, note, nbuckets will be round up to 2^n
 */
pt_cqueue_t *
create_cqueue(uint32_t nbuckets, pt_time_t delta_time)
{
    pt_cqueue_t *cq = PT_CALLOC(1, sizeof(pt_cqueue_t));    
    if (cq == NULL) {
        return NULL;    
    }

    cq->nbuckets = 2;
    while ((nbuckets = nbuckets >> 1)) {
        cq->nbuckets <<= 1;    
    }

    cq->buckets = PT_CALLOC(cq->nbuckets, sizeof(pt_dlist_t));
    if (cq->buckets == NULL) {
        PT_FREE(cq);    
        return NULL;
    }
    cq->current = 0;
    cq->delta_time = delta_time;
    cq->elapse_time = 0;
    cq->start = start;
    cq->poll_next = poll_next;
    cq->add_node = add_node;
    cq->remove_node = remove_node;
    cq->add_object = add_object;

    PT_DEBUG("%s %d buckets %lld delta time \n", __FUNCTION__, 
        cq->nbuckets, (long long)delta_time);
    return cq;
}


/*
 * Destroy the calendar queue, note: caller must dereference the opaque pointers
 * in the pt_cqueue_node_t
 */
void 
destroy_cqueue(pt_cqueue_t *cq)
{
    if (cq->buckets) {
        uint32_t i = 0;
        for ( i = 0; i < cq->nbuckets; i++) {
            pt_dlist_t *dl = &(cq->buckets[i]);
            pt_dnode_t *node = NULL;
            while((node = PT_DLIST_REMOVE_HEAD(dl)) != NULL) {
                PT_FREE((pt_cqueue_node_t*)node);
            }
        }
        PT_FREE(cq->buckets);    
    }    

    PT_FREE(cq);
}

