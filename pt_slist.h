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
#ifndef __PT_SLIST_H__
#define __PT_SLIST_H__

#include "pt_defs.h"
#include "pt_node.h"

/*
 * slist
 *
 * -----------------------------------------------------
 *  slist  
 *   |    
 *   V  
 * node_1 -> node_2 -> node_3 -> ... -> node_n -> NULL
 * -----------------------------------------------------
 */

typedef struct pt_slist_s
{
    pt_snode_t *head;  
} pt_slist_t;

#define PT_SLIST_HEAD(_slist_) ((_slist_)->head)

/* init a single linked list */
static inline
void 
PT_SLIST_INIT(pt_slist_t *l)
{
    l->head = NULL;
}

/* add a node to a single linked list */
static inline 
void
PT_SLIST_ADD(pt_slist_t *l, pt_snode_t *n)
{
    n->next = l->head;    
    l->head = n;
}

/* remove a node from a head of a single linked list */
static inline 
pt_snode_t *
PT_SLIST_REMOVE_HEAD(pt_slist_t *l)
{
    pt_snode_t *n = NULL;
    PT_ASSERT(l != NULL);

    if (l->head != NULL) {
        n = l->head;
        l->head = n->next;
        return n;
    }

    return NULL;
}


/* count number of nodes in a single linked list */
static inline
uint32_t 
PT_SLIST_COUNT(pt_slist_t *l)
{
    uint32_t count = 0;  
    pt_snode_t *n = NULL;

    PT_ASSERT(l != NULL);
    
    n = PT_SLIST_HEAD(l);

    while( n != NULL) {
        count++;
        n = PT_SNODE_NEXT(n);
    }
    return count;
}

/* reverse a single linked list */
static inline 
void
PT_SLIST_REVERSE(pt_slist_t *l)
{
    pt_snode_t *before = NULL;
    pt_snode_t *current = NULL;
    pt_snode_t *next = NULL;

    PT_ASSERT(l != NULL);
    if (l->head == NULL || l->head->next == NULL) {
        /* list is empty or only one element in the list */
        return;    
    }

    /* at least two elements */
    before = l->head;
    current = before->next;
    before->next = NULL;

    while(current->next != NULL) {
        next = current->next;
        current->next = before;
        before = current;
        current = next;
    }
    current->next = before;
    l->head = current;
}

/* insert after a node */
static inline
void 
PT_SLIST_INSERT_AFTER(pt_snode_t *after, pt_snode_t *n)
{
    n->next = after->next; 
    after->next = n;
}

/* sort the list */
#define NODE_VALUE(_n_, _ofs_)    (*(uint32_t*)((uint8_t *)(_n_) + _ofs_))
static inline
void
PT_SLIST_SORT(pt_slist_t *list, uint16_t offset)
{
    pt_snode_t *node = NULL;
    pt_snode_t *pivot = NULL;
    pt_slist_t l;   /* less list             */
    pt_slist_t g;   /* greater or equal list */

    PT_SLIST_INIT(&l);
    PT_SLIST_INIT(&g);

    node = PT_SLIST_HEAD(list);
    while(node != NULL) {
        node = PT_SNODE_NEXT(node);    
    }

    pivot = PT_SLIST_REMOVE_HEAD(list); 
    if (pivot == NULL) {
        return;    
    }

    while((node = PT_SLIST_REMOVE_HEAD(list)) != NULL) {
        if (NODE_VALUE(node, offset) < NODE_VALUE(pivot, offset)) {
            PT_SLIST_ADD(&l, node);   
        }    
        else {
            PT_SLIST_ADD(&g, node);    
        }
    }

    PT_SLIST_SORT(&l, offset);
    PT_SLIST_SORT(&g, offset);
    node = PT_SLIST_HEAD(&l);
    if (node == NULL) {
        /* less list is empty */    
        PT_SLIST_HEAD(list) = pivot;
        PT_SNODE_NEXT(pivot) = PT_SLIST_HEAD(&g);
        return;
    }
    while(PT_SNODE_NEXT(node) != NULL) {
        node = PT_SNODE_NEXT(node);
    }

    PT_SLIST_HEAD(list) = PT_SLIST_HEAD(&l);
    PT_SNODE_NEXT(node) = pivot;
    PT_SNODE_NEXT(pivot) = PT_SLIST_HEAD(&g);
}

#endif /* !__PT_SLIST_H__*/
