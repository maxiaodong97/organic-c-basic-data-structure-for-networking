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
#ifndef __PT_CQUEUE_H__
#define __PT_CQUEUE_H__

#include "pt_defs.h"
#include "pt_dlist.h"

/*
 * cqueue - calendar queue 
 *
 * A data structure that is used to trigger timer events
 * 
 * ----------------------------------------------------------------------------
 *          |
 *          V
 *  +----+----+----+----+----+----+----+----+
 *  | 0  |  1 |  2 |  3 |  4 |  ....   | n-1|  
 *  +----+----+----+----+----+----+----+----+
 *               |
 *               V
 *             dlist
 *       
 *  An array of n, each is a point to a double listed sorted list. A current 
 *  index is moving in a circle, its double linked list will be checked for 
 *  the node to be timeouted.
 * ----------------------------------------------------------------------------
 */
typedef struct pt_cqueue_node_s pt_cqueue_node_t;

typedef uint64_t pt_time_t;

#define PT_TIME_LT(_a_, _b_)  ((int32_t)(_a_ - _b_) < 0)
#define PT_TIME_LE(_a_, _b_)  ((int32_t)(_a_ - _b_) <= 0)
#define PT_TIME_GT(_a_, _b_)  ((int32_t)(_a_ - _b_) > 0)
#define PT_TIME_GE(_a_, _b_)  ((int32_t)(_a_ - _b_) >= 0)

typedef struct pt_cqueue_s
{
    pt_dlist_t *buckets;            /* circular buckets                 */
    uint32_t nbuckets;              /* number of buckets                */
    uint32_t current;               /* current bucket index             */
    pt_time_t delta_time;           /* time resolution                  */
    pt_time_t elapse_time;          /* elapse time of current poll      */

    /*
     * start the calendar queue "now"
     */
    void (*start)(struct pt_cqueue_s *cq, pt_time_t now);

    /*
     * poll a object, node will be removed from the queue, caller need to insert
     * it back. 
     */
    pt_cqueue_node_t* (*poll_next)(struct pt_cqueue_s *cq, pt_time_t cur_time);

    /*
     * add a opaque object to the calendar queue, this object event will be
     * triggered at absolute time "abs_time". 
     */
    pt_cqueue_node_t* (*add_object)(struct pt_cqueue_s *cq, void *object, 
                pt_time_t abs_time);

    /*
     * add a cqueue_node back to calendar queue, caller need to set what is the
     * next timeout time in cq_node->timeout.
     */
    void (*add_node)(struct pt_cqueue_s *cq, pt_cqueue_node_t *cq_node);

    /*
     * remove a cqueue_node from calendar queue
     */
    void (*remove_node)(struct pt_cqueue_s *cq, pt_cqueue_node_t *cq_node);

} pt_cqueue_t;

/*
 * Calendar queue node in above double linked list
 */
struct pt_cqueue_node_s
{
    pt_dnode_t node;            /* nodes to be double linked            */ 
    struct pt_cqueue_s *cq;     /* point to its parent                  */ 
    pt_time_t timeout;          /* absolute timeout value, sorted key   */
    uint32_t bucket_index;      /* the bucket index of this node belongs*/
    void *ptr;                  /* opaque pointer for object            */
};

/* 
 * Create a calendar queue with nbuckets, note, nbuckets will be round up 
 * to 2^n, move to next bucket for every "delta_time" time elapse
 */
pt_cqueue_t *
create_cqueue(uint32_t nbuckets, pt_time_t delta_time);

/*
 * Destroy the calendar queue, note: caller must dereference the opaque 
 * pointers in the pt_cqueue_node_t
 */
void 
destroy_cqueue(pt_cqueue_t *cq);

#endif /* !__PT_CQUEUE_H__ */
