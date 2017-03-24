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
#ifndef __PT_VECTOR_H__
#define __PT_VECTOR_H__

#include "pt_defs.h"

typedef struct pt_vector_s
{
    void *array;                /* vector holder        */
    uint32_t array_size;        /* vector size          */
    uint32_t nelements;         /* number of elements   */
    uint32_t elsize;            /* element size         */
    uint32_t resize_factor;     /* % increase, eg: 20   */
} pt_vector_t;

#define PT_VECTOR_COUNT(_vector_)           ((_vector_)->nelements)
#define PT_VECTOR_ELEMENT_SIZE(_vector_)    ((_vector_)->elsize)

/* allocate  vector, return 0 if succeed */
static inline
int 
PT_VECTOR_ALLOC(pt_vector_t *v, 
    uint32_t nelements, 
    uint32_t elsize, 
    uint32_t resize_factor)
{
    PT_ASSERT(v != NULL);

    v->array_size = nelements * elsize * (100 + resize_factor) / 100;
    v->array = PT_MALLOC(v->array_size);
    if (v->array == NULL) {
        return -1;
    }
    v->nelements = 0;
    v->elsize = elsize;
    v->resize_factor = resize_factor;
    return 0;
}

/* free a vector, note, v itself is not freed */
static inline
void
PT_VECTOR_FREE(pt_vector_t *v)
{
    PT_ASSERT(v != NULL);

    PT_FREE(v->array);
}

/* add an element to a vector */
static inline
int 
PT_VECTOR_ADD(pt_vector_t *v, void *elp, uint32_t elsize)
{
    PT_ASSERT(v!= NULL);

    if (v->elsize != elsize) {
        return -1;    
    }

    if ((v->nelements + 1) * v->elsize >= v->array_size) {
        void *new_array = NULL;
        uint32_t new_size = (v->nelements + 1) * v->elsize * 
                    (100 + v->resize_factor) / 100;
        new_array = PT_REALLOC(v->array, new_size);
        if (new_array == NULL) {
            return -1;    
        }
        v->array = new_array;
        v->array_size = new_size;
    }    

    PT_MEMCPY(v->array + v->nelements * v->elsize, 
        elp, elsize);
    v->nelements++;
    return 0;
}

/* return No. i element address */
static inline
void *
PT_VECTOR_GET_I(pt_vector_t *v, uint32_t i)
{
    PT_ASSERT(v != NULL);

    if (i >= v->nelements) {
        return NULL;    
    }

    return (v->array + i * v->elsize);
}

#endif /* __PT_VECTOR_H__ */
