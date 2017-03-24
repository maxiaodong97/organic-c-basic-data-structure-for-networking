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
#ifndef __PT_DLIST_H__
#define __PT_DLIST_H__

#include "pt_defs.h"
#include "pt_node.h"

/*
 * dlist
 *
 * -----------------------------------------------------
 *  head                                     tail   
 *   |                                        |
 *   V                                        V
 * node_1 <-> node_2 <-> node_3 <-> ... <-> node_n -> NULL
 * -----------------------------------------------------
 */

typedef struct pt_dlist_s
{
    pt_dnode_t *head;  
    pt_dnode_t *tail;  
} pt_dlist_t;

#define PT_DLIST_HEAD(_dlist_)  ((_dlist_)->head)
#define PT_DLIST_TAIL(_dlist_)  ((_dlist_)->tail)

/* init double linked list */
static inline
void 
PT_DLIST_INIT(pt_dlist_t *l)
{
    l->head = NULL;
    l->tail = NULL;
}

/* add a node to the head of a double linked list */
static inline 
void
PT_DLIST_ADD_HEAD(pt_dlist_t *l, pt_dnode_t *n)
{
    PT_ASSERT(l != NULL);
    PT_ASSERT(n->next == NULL);
    PT_ASSERT(n->prev == NULL);

    if (l->head == NULL) {
        PT_ASSERT(l->tail == NULL);
        l->head = l->tail = n;
        return;
    }

    n->next = l->head;
    l->head->prev = n;
    l->head = n;
}

/* add a node to the tail of a double linked list */
static inline 
void
PT_DLIST_ADD_TAIL(pt_dlist_t *l, pt_dnode_t *n)
{
    PT_ASSERT(l != NULL);
    PT_ASSERT(n->next == NULL);
    PT_ASSERT(n->prev == NULL);

    if (l->tail == NULL) {
        PT_ASSERT(l->head == NULL);
        l->head = l->tail = n;
        return;
    }

    l->tail->next = n;
    n->prev = l->tail;
    l->tail = n;
}

/* add a node before another node of a double linked list */
static inline 
void
PT_DLIST_ADD_BEFORE(pt_dlist_t *l, pt_dnode_t *before, pt_dnode_t *n)
{
    PT_ASSERT(l != NULL);
    PT_ASSERT(n->next == NULL);
    PT_ASSERT(n->prev == NULL);
    PT_ASSERT(before != NULL);

    if (before == l->head) {
        PT_DLIST_ADD_HEAD(l, n);    
        return;
    }

    PT_ASSERT(before->prev != NULL);
    
    n->next = before;
    n->prev = before->prev;
    before->prev->next = n;
    before->prev = n;
}

/* add a node after another node of a double linked list */
static inline 
void
PT_DLIST_ADD_AFTER(pt_dlist_t *l, pt_dnode_t *after, pt_dnode_t *n)
{
    PT_ASSERT(l != NULL);
    PT_ASSERT(n->next == NULL);
    PT_ASSERT(n->prev == NULL);
    PT_ASSERT(after != NULL);

    if (after == l->tail) {
        PT_DLIST_ADD_TAIL(l, n);    
        return;
    }

    PT_ASSERT(after->next != NULL);
    n->next = after->next;
    n->prev = after;
    after->next->prev = n;
    after->next = n;
}

/* remove a node from the head of a double linked list */
static inline
pt_dnode_t *
PT_DLIST_REMOVE_HEAD(pt_dlist_t *l)
{
    pt_dnode_t *n = NULL;
    PT_ASSERT(l != NULL);

    if (l->head == NULL) {
        PT_ASSERT(l->tail == NULL);    
        return NULL;
    }

    n = l->head;
    if (l->head == l->tail) {
        l->head = l->tail = NULL;    
        return n;
    }

    l->head = n->next;
    n->next->prev = NULL;
    return n;
}

/* remove a node from the tail of a double linked list */
static inline
pt_dnode_t *
PT_DLIST_REMOVE_TAIL(pt_dlist_t *l)
{
    pt_dnode_t *n = NULL;
    PT_ASSERT(l != NULL);

    if (l->tail == NULL) {
        PT_ASSERT(l->head == NULL);    
        return NULL;
    }

    n = l->tail;
    if (l->head == l->tail) {
        l->head = l->tail = NULL;    
        return n;
    }

    l->tail = n->prev;
    n->prev->next = NULL;
    return n;
}

/* check if a node belongs to a double linked list */
static inline
bool_t
PT_DLIST_HAS_NODE(pt_dlist_t *l, pt_dnode_t *n)
{
    pt_dnode_t *h = NULL;
    PT_ASSERT(l != NULL);
    PT_ASSERT(n != NULL);

    h = l->head;    
    while(h != NULL) {
        if (h == n)  {
            return TRUE;    
        }   
        h = PT_DNODE_NEXT(h);
    }
    return FALSE;
}

/* remove a node from a double linked list */
static inline
pt_dnode_t *
PT_DLIST_REMOVE_NODE(pt_dlist_t *l, pt_dnode_t *n)
{
    PT_ASSERT(l != NULL);
    PT_ASSERT(n != NULL);

    if (l->tail == NULL) {
        PT_ASSERT(l->head == NULL);    
        return NULL;
    }

    if (l->head == n) {
        return PT_DLIST_REMOVE_HEAD(l);    
    }

    if (l->tail == n) {
        return PT_DLIST_REMOVE_TAIL(l);    
    }

    n->next->prev = n->prev;
    n->prev->next = n->next;
    return n;
}

/* count number of nodes in a double linked list */
static inline
uint32_t 
PT_DLIST_COUNT(pt_dlist_t *l)
{
    uint32_t count = 0;  
    pt_dnode_t *n = NULL;

    PT_ASSERT(l != NULL);
    
    n = PT_DLIST_HEAD(l);

    while( n != NULL) {
        count++;
        n = PT_DNODE_NEXT(n);
    }
    return count;
}

/* append another list at the end of the current list */
static inline
void
PT_DLIST_APPEND(pt_dlist_t *l, pt_dlist_t *n)
{
    PT_ASSERT(l != NULL);
    if (l->tail == NULL) {
        PT_ASSERT(l->head == NULL);    
        l->head = n->head;
        l->tail = n->tail;
        return;
    }

    if (n->head == NULL) {
        PT_ASSERT(n->tail == NULL);    
        return;
    }

    l->tail->next = n->head;
    n->head->prev = l->tail;
    l->tail = n->tail;
}



#endif /* !__PT_SLIST_H__*/
