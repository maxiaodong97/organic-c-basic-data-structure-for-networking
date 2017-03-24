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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pt_slist.h"
#include "pt_vector.h"
#include "pt_dlist.h"
#include "pt_queue.h"
#include "pt_hash.h"
#include "pt_cqueue.h"

#define LIST_COUNT 5
void 
test_slist(void)
{
    pt_slist_t l;
    pt_snode_t *node = NULL;
    int i = 0;
    struct inode_s {
        pt_snode_t node; /* must be the first element of the object */
        int data;
    };

    printf("+------------------------------------------+\n");
    printf("| testing pt_slist_t....                   |\n");
    printf("+------------------------------------------+\n");

#define PRINT_SLIST(l) \
    {                                                   \
        node = PT_SLIST_HEAD(&l);                       \
        while(node != NULL) {                           \
            struct inode_s *n = (struct inode_s*)node;  \
            printf("%d ", n->data);                     \
            node = PT_SNODE_NEXT(node);                 \
        }                                               \
        printf("\n");                                   \
    }

    PT_SLIST_INIT(&l);

    for (i = 0; i < LIST_COUNT ; i++) {
        struct inode_s *n = (struct inode_s*)
                    calloc(1, sizeof(struct inode_s));       
        PT_ASSERT(n != NULL);
        n->data = i;
        PT_SLIST_ADD(&l, &n->node);
    }

    for (; i > 0 ; i--) {
        struct inode_s *n = (struct inode_s*)
                    calloc(1, sizeof(struct inode_s));       
        PT_ASSERT(n != NULL);
        n->data = i;
        PT_SLIST_ADD(&l, &n->node);
    }

    printf("the list is: ");
    PRINT_SLIST(l);

    printf("sort the list: ");
    PT_SLIST_SORT(&l, (uint16_t)sizeof(pt_snode_t));
    PRINT_SLIST(l);

    printf("reverse it: ");
    PT_SLIST_REVERSE(&l);
    PRINT_SLIST(l);

    node = PT_SLIST_REMOVE_HEAD(&l);
    while(node != NULL) {
        free(node);        
        node = PT_SLIST_REMOVE_HEAD(&l);
    }
}

void
test_dlist(void)
{
    pt_dlist_t l;
    pt_dlist_t r;
    pt_dnode_t *node = NULL;
    int i = 0;
    struct inode_s {
        pt_dnode_t node; /* must be the first element of the object */
        int data;
    };

    printf("+------------------------------------------+\n");
    printf("| testing pt_dlist_t....                   |\n");
    printf("+------------------------------------------+\n");

#define PRINT_DLIST(l, b_reverse) \
    {                                                                     \
        node = b_reverse ? PT_DLIST_TAIL(&l) : PT_DLIST_HEAD(&l);         \
        while(node != NULL) {                                             \
            struct inode_s *n = (struct inode_s*)node;                    \
            printf("%d ", n->data);                                       \
            node = b_reverse ? PT_DNODE_PREV(node): PT_DNODE_NEXT(node);  \
        }                                                                 \
        printf("\n");                                                     \
    }

    PT_DLIST_INIT(&l);
    PT_DLIST_INIT(&r);

    for (i = 0; i < LIST_COUNT ; i++) {
        struct inode_s *n = (struct inode_s*)
                    calloc(1, sizeof(struct inode_s));       
        PT_ASSERT(n != NULL);
        n->data = i;
        if (i % 2 == 0) {
            PT_DLIST_ADD_TAIL(&l, &n->node);
        } 
        else {
            PT_DLIST_ADD_HEAD(&l, &n->node);
        }
    }

    for (i = 0; i < LIST_COUNT ; i++) {
        struct inode_s *n = (struct inode_s*)
                    calloc(1, sizeof(struct inode_s));       
        PT_ASSERT(n != NULL);
        n->data = i;
        if (i % 2 == 0) {
            PT_DLIST_ADD_TAIL(&r, &n->node);
        } 
        else {
            PT_DLIST_ADD_HEAD(&r, &n->node);
        }
    }

    printf("count = %d\n", PT_DLIST_COUNT(&l));
    PRINT_DLIST(l, FALSE);

    printf("reverse traverse: ");
    PRINT_DLIST(l, TRUE);

    printf("append a new list: ");
    PT_DLIST_APPEND(&l, &r);
    PRINT_DLIST(l, TRUE);

    for (i = 0; i < LIST_COUNT; i++) {
        printf("After remove %d : ", i);
        node = PT_DLIST_HEAD(&l);
        while(node != NULL) {
            struct inode_s *n = (struct inode_s*)node; 
            pt_dnode_t *next = PT_DNODE_NEXT(node);
            if (n->data == i) {
                PT_DLIST_REMOVE_NODE(&l, node);
                free(node);        
            }
            node = next;
        }
        PRINT_DLIST(l, FALSE);
    }
}

void 
test_queue(void)
{
    pt_queue_t q; 
    pt_snode_t *node = NULL;
    int i = 0;
    struct inode_s {
        pt_snode_t node; /* must be the first element of the object */
        int data;
    };

    printf("+------------------------------------------+\n");
    printf("| testing pt_queue_t....                   |\n");
    printf("+------------------------------------------+\n");

#define PRINT_QUEUE(q) \
    {                                                   \
        node = PT_QUEUE_HEAD(&q);                       \
        while(node != NULL) {                           \
            struct inode_s *n = (struct inode_s*)node;  \
            printf("%d ", n->data);                     \
            node = PT_SNODE_NEXT(node);                 \
        }                                               \
        printf("\n");                                   \
    }

    PT_QUEUE_INIT(&q);

    for (i = 0; i < LIST_COUNT ; i++) {
        struct inode_s *n = (struct inode_s*)
                    calloc(1, sizeof(struct inode_s));       
        PT_ASSERT(n != NULL);
        n->data = i;
        if (i % 2 == 0) {
            PT_QUEUE_ADD_TAIL(&q, &n->node);
        } 
        else {
            PT_QUEUE_ADD_HEAD(&q, &n->node);
        }
    }

    printf("count = %d: ", PT_QUEUE_COUNT(&q));
    PRINT_QUEUE(q);

    node = PT_QUEUE_PEEK_HEAD(&q);
    if (node != NULL) {
        struct inode_s *n = (struct inode_s*)node; 
        printf("peek head = %d\n", n->data);
    }

    node = PT_QUEUE_PEEK_TAIL(&q);
    if (node != NULL) {
        struct inode_s *n = (struct inode_s*)node; 
        printf("peek tail = %d\n", n->data);
    }


    while (PT_QUEUE_PEEK_HEAD(&q) != NULL) {
        node = PT_QUEUE_REMOVE_HEAD(&q);
        if (node != NULL) {
            struct inode_s *n = (struct inode_s*)node; 
            printf("After remove head %d : ", n->data);
            PRINT_QUEUE(q);
            free(node);
        }

        node = PT_QUEUE_REMOVE_TAIL(&q);
        if (node != NULL) {
            struct inode_s *n = (struct inode_s*)node; 
            printf("After remove tail = %d : ", n->data);
            PRINT_QUEUE(q);
            free(node);
        }
    }
}

void 
test_vector(void)
{
    pt_vector_t v;    
    int i = 0;
    int ret = 0; 

    printf("+------------------------------------------+\n");
    printf("| testing pt_vector_t....                  |\n");
    printf("+------------------------------------------+\n");

    ret = PT_VECTOR_ALLOC(&v, LIST_COUNT, sizeof(int), 50);
    PT_ASSERT(ret == 0);

#define PRINT_VECTOR(v)     \
    {                                                       \
        int __i = 0;                                        \
        for (__i = 0; __i < PT_VECTOR_COUNT(v); __i++) {    \
            int *__p = (int *)PT_VECTOR_GET_I(v, __i);      \
            if (__p) {                                      \
                printf("[%d] = %d\n", __i, *__p);          \
            }                                               \
        }                                                   \
    }

    for (i = 0; i < LIST_COUNT; i++) {
        PT_VECTOR_ADD(&v, &i, sizeof(int));    
    }
    PRINT_VECTOR(&v);

    for (i = 0; i < LIST_COUNT; i++) {
        PT_VECTOR_ADD(&v, &i, sizeof(int));    
    }
    PRINT_VECTOR(&v);
    PT_VECTOR_FREE(&v);
}

void 
test_hash(void)
{
    pt_hash_t *hp = create_hash(100);
    pt_hash_iter_t *iter = NULL;
    int key = 0;
    int val = 0;

    void *k = NULL;
    void *v = NULL;
    uint16_t sk  = 0; 
    uint16_t sv = 0;

    printf("+------------------------------------------+\n");
    printf("| testing pt_hash_t....                    |\n");
    printf("+------------------------------------------+\n");

    hp->debug_print(hp);

    for (key = 0; key < 10; key++) {
        val = key + 0xFF;
        hp->insert(hp, &key, sizeof(key), &val, sizeof(val)); 
    }

    hp->debug_print(hp);

    iter = hp->iter_construct(hp);

    while(hp->iter_next(iter, (void**)&k, &sk, (void**)&v, &sv) == 0) {
        printf("iter: key = %d, value = %d\n", *(int*)k, *(int*)v);    
    }
    hp->iter_destruct(iter);

    for (key = 0; key < 11; key++) {
        if (hp->find(hp, &key, sizeof(key), &v, &sv) == 0) {
            printf("find value %d for key = %d\n", *(int*)v, key);    
        }
        else {
            printf("key = %d not found\n", key);    
        }
    }

    destroy_hash(hp);
}

void 
test_cqueue(void)
{
    pt_cqueue_t *cq = create_cqueue(7, 1);

    struct object_s {
        char name[32];    
        uint32_t timeout;    
    };

#define NUM_OF_OBJECTS 5

    struct object_s obj[NUM_OF_OBJECTS]; 
    int i = 0;
    pt_time_t cur = 0;

    printf("+------------------------------------------+\n");
    printf("| testing pt_cqueue_t....                  |\n");
    printf("+------------------------------------------+\n");

    if (cq == NULL) {
        printf("failed to create cqueue.\n");    
        return;
    }

    for (i = 1 ; i < NUM_OF_OBJECTS; i++) {
        sprintf(obj[i].name, "object_%d", i);    
        obj[i].timeout = i;
        cq->add_object(cq, &(obj[i]), obj[i].timeout);    
    }

#define PRINT_CQUEUE(cq) \
    { \
        uint32_t __i = 0;        \
        printf("--------------------------------\n"); \
        for (__i = 0 ; __i < cq->nbuckets; __i++) {    \
            pt_dlist_t *dl = &(cq->buckets[__i]); \
            pt_dnode_t *node = PT_DLIST_HEAD(dl); \
            printf("%s bucket %d: \n", cq->current == __i? "->": "  ", __i); \
            while(node != NULL) { \
                pt_cqueue_node_t *cq_node = (pt_cqueue_node_t*)node; \
                struct object_s *obj = (struct object_s *)cq_node->ptr; \
                printf("    %s timeout value %d\n", obj->name, obj->timeout); \
                node = PT_DNODE_NEXT(node); \
            } \
        } \
        printf("--------------------------------\n"); \
    }
    
    PRINT_CQUEUE(cq);

    cq->start(cq, 0);

    i = 0;
    while(i++ < 2 * NUM_OF_OBJECTS) {
        pt_cqueue_node_t *cq_node = NULL;
        while((cq_node = cq->poll_next(cq, cur)) != NULL) {
            struct object_s *obj = (struct object_s*)cq_node->ptr;
            printf("at time %lld, [%s] triggered!\n", (long long)cur, obj->name);
            cq_node->timeout = cur + obj->timeout;
            cq->add_node(cq, cq_node);
        }
        PRINT_CQUEUE(cq);
        cur += 3;
    }

    destroy_cqueue(cq);
}

int main(int argc, char**argv) {
    test_slist();
    test_dlist();
    test_queue();
    test_vector();
    test_hash();
    test_cqueue();
    return 0;
}
